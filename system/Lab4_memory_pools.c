#include <xinu.h>

#define MaxHeap (20 * (1 << 20))
#define DEBUG 0

// implicit Link定义宏
#define MinimumBlock 32
#define WSize 4
#define DSize 8
#define Alignment 8
#define Padding 64
#define Round 128
#define Align(size) (((size) + (Alignment-1)) & ~0x7)
#define Read(x) (*(unsigned *)(x))
#define Write(x, v) (*(unsigned *)(x) = (v))
#define Length(x) (Read(x) & ~0x7)
#define IsUsed(x) (Read(x) & 1)
#define GetHead(x) ((char*) (x) - WSize)
#define GetFoot(x) ((char*) (x) + Length(GetHead(x)) - DSize)
#define Next(x) ((char*) (x) + Length((char*) (x) - WSize))
#define Prev(x) ((char*) (x) - Length((char*) (x) - DSize))

// BST定义宏
#define Parent(x) (*(Node*)(x))
#define LC(x) (*(Node*)(x + DSize))
#define RC(x) (*(Node*)(x + 2 * DSize))
#define IsRoot(x) !Parent(x)
#define IsLChild(x) (Parent(x) && ((x) == LC(Parent(x))))
#define IsRChild(x) (Parent(x) && ((x) == RC(Parent(x))))
#define FromParentTo(x) (IsRoot(x)? root : (IsLChild(x)? &LC(Parent(x)) : &RC(Parent(x))))

// Red-Black Tree定义宏
#define Black 0
#define Red 1
#define Get(x) (*(int*) (x))
#define Color(x) ((x)? (Get(GetHead(x)) & 0x2) >> 1 : 0)
#define SetColor(x, v) ((x) ? ((v)? (Get(GetHead(x)) |= 0x2) : (Get(GetHead(x)) &= ~0x2)) : (x))

typedef void *Address;
typedef uint32 Node;

// Red-Black Tree内部函数
int cmp(Node a, Node b);

Node getNext(Node u);

void swap(Node *a, Node *b);

void swapNode(Node a, Node b);

void takeOff(Node u);

void RBtree_insert(Address address);

void RBtree_delete(Address address);

Node rotateAt(Node u);

Node connect34(Node a, Node b, Node c, Node t0, Node t1, Node t2, Node t3);

void solveDoubleRed(Node x);

void solveDoubleBlack(Node r);

void print(Node u);

int traverse(Node u);

// implicit Link 函数

uint32 power2(uint32 x);

void *extend(uint32 size);

void *findFit(Node u, uint32 size);

void *placeHead(void *ptr, int size);

void *placeTail(void *p, int size);

void *coalesce(void *bp);

// DEGUB函数
int checkNext(void *ptr, int size);

void mm_checkblock(int verbose, const char *func, void *bp);

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
    Node *root = mem_sbrk(DSize);
    uint32 size = Align(WSize * 0x4);
    void *head;
    if ((head = mem_sbrk(size)) == (void *) -1)
        return -1;

    *root = 0;
    memset(head, 0, PAGE_SIZE);
    Write(head + WSize, 0x8 | 0x1);
    Write(head + WSize * 2, 0x8 | 0x1);
    Write(head + WSize * 3, 0x1);
    head += WSize * 2;

    if (!extend(PAGE_SIZE))
        return -1;
    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */

void *mm_malloc(uint32 size) {
    if (DEBUG)
        printf("Malloc: %d\n", (int) size);
    Node *root = mem_heap_lo();
    uint32 newSize = Align(size + WSize * 2);
    uint32 pow2 = power2(newSize);
    pow2 += DSize;
    if (!size) return NULL;

    void *ptr = findFit(*root, newSize);
    if (ptr) {
        if (pow2 >= Round && Length(GetHead(ptr)) >= pow2 && pow2 - newSize <= Padding)
            ptr = placeHead(ptr, pow2);
        else
            ptr = placeTail(ptr, newSize);
        return ptr;
    }

    uint32 extendSize = ((size == 64)? (PAGE_SIZE * 2) : PAGE_SIZE);
    if ((ptr = extend(newSize > extendSize ? newSize : extendSize)) != NULL) {
        if (pow2 >= Round && Length(GetHead(ptr)) >= pow2 && pow2 - newSize <= Padding)
            ptr = placeHead(ptr, pow2);
        else
            ptr = placeTail(ptr, newSize);
    }
    return ptr;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
    if (DEBUG)
        printf("Free: %zx\n", (uint32) ptr);
    uint32 size = Length(GetHead(ptr));
    Write(GetHead(ptr), size | 0x0);
    Write(GetFoot(ptr), size | 0x0);
    mm_checkblock(1, "", ptr);

    coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, uint32 size) {
    uint32 oldSize, newSize;
    void *oldPtr, *newPtr;

    if (ptr == NULL)
        return mm_malloc(size);
    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    newSize = Align(size + 2 * WSize);
    oldSize = Length(GetHead(ptr));
    oldPtr = ptr;
    if (oldSize >= newSize) {
        if (oldSize - newSize >= MinimumBlock)
            oldPtr = placeTail(oldPtr, newSize);
        return oldPtr;
    } else {
        if (checkNext(oldPtr, newSize)) return oldPtr;
        Node *root = mem_heap_lo();
        if (!Length(GetHead(Next(oldPtr)))) {
            uint32 reSize = newSize - oldSize;
            if ((newPtr = mem_sbrk(reSize)) == NULL)
                return NULL;
            Write(GetHead(oldPtr), newSize | 0x1);
            Write(GetFoot(oldPtr), newSize | 0x1);
            Write(GetFoot(oldPtr) + WSize, 0x1);
            return oldPtr;
        }
        if ((newPtr = findFit(*root, newSize)) == NULL) {
            if ((newPtr = extend(newSize > PAGE_SIZE ? newSize : PAGE_SIZE)) == NULL)
                return NULL;
        }
        newPtr = placeTail(newPtr, newSize);
        memcpy(newPtr, oldPtr, oldSize - 2 * WSize);
        mm_free(oldPtr);
        return newPtr;
    }
}

uint32 power2(uint32 x) {
    int pos = 0, i = 0;
    for (i = 0; x >> i; i ++ )
        pos = i;
    return 1 << (pos + 1);
}

void *extend(uint32 size) {
    uint32 newSize = Align(size);
    void *ptr;
    if ((ptr = mem_sbrk(newSize)) == (void *) -1)
        return NULL;
    Write(GetHead(ptr), newSize);
    Write(GetFoot(ptr), newSize);
    mm_checkblock(DEBUG, "", ptr);
    Write(GetHead(Next(ptr)), 0x1 | 0x0);
    return coalesce(ptr);
}

void *findFit(Node u, uint32 size) {
    if (!u) return NULL;
    if (Length(GetHead(u)) < size) return findFit(RC(u), size);
    Node w = (Node) findFit(LC(u), size);
    if (w && cmp(u, w)) return (void *) w;
    return (void *) u;
}

void *placeHead(void *ptr, int size) {
    int sum = Length(GetHead(ptr));
    int last = sum - size;
    mm_checkblock(DEBUG, "", ptr);
    if (ptr) RBtree_delete(ptr);

    if (last >= MinimumBlock) {
        Write(GetHead(ptr), size | 0x1);
        Write(GetFoot(ptr), size | 0x1);
        void *next = ptr + size;
        Write(GetHead(next), last | 0x0);
        Write(GetFoot(next), last | 0x0);
        RBtree_insert(next);
        mm_checkblock(DEBUG, "", ptr);
        mm_checkblock(DEBUG, "", next);
    } else {
        Write(GetHead(ptr), sum | 0x1);
        Write(GetFoot(ptr), sum | 0x1);
        mm_checkblock(DEBUG, "", ptr);
    }
    return ptr;
}

void *placeTail(void *ptr, int size) {
    int sum = Length(GetHead(ptr));
    int last = sum - size;
    mm_checkblock(DEBUG, "", ptr);
    if (ptr) RBtree_delete(ptr);

    if (last >= MinimumBlock) {
        void *next = ptr + last;
        Write(GetHead(next), size | 0x1);
        Write(GetFoot(next), size | 0x1);
        Write(GetHead(ptr), last | 0x0);
        Write(GetFoot(ptr), last | 0x0);
        RBtree_insert(ptr);
        ptr = next;
        mm_checkblock(DEBUG, "", ptr);
        mm_checkblock(DEBUG, "", next);
    } else {
        Write(GetHead(ptr), sum | 0x1);
        Write(GetFoot(ptr), sum | 0x1);
        mm_checkblock(DEBUG, "", ptr);
    }
    return ptr;
}

void *coalesce(void *ptr) {
    int prev = IsUsed(GetHead(Prev(ptr)));
    int next = IsUsed(GetHead(Next(ptr)));
    uint32 size = Length(GetHead(ptr));

    if (prev && next) {
        if (Length(GetHead(ptr)) >= MinimumBlock)
            RBtree_insert(ptr);
        mm_checkblock(DEBUG, "", ptr);
        return ptr;
    } else if (prev && !next) {
        size += Length(GetHead(Next(ptr)));
        mm_checkblock(DEBUG, "", Next(ptr));
        if (Length(GetHead(Next(ptr))) >= MinimumBlock)
            RBtree_delete(Next(ptr));
    } else if (!prev && next) {
        size += Length(GetHead(Prev(ptr)));
        mm_checkblock(DEBUG, "", Prev(ptr));
        if (Length(GetHead(Prev(ptr))) >= MinimumBlock)
            RBtree_delete(Prev(ptr));
        ptr = Prev(ptr);
    } else {
        size += Length(GetHead(Prev(ptr))) + Length(GetHead(Next(ptr)));
        mm_checkblock(DEBUG, "", Next(ptr));
        mm_checkblock(DEBUG, "", Prev(ptr));
        if (Length(GetHead(Next(ptr))) >= MinimumBlock)
            RBtree_delete(Next(ptr));
        if (Length(GetHead(Prev(ptr))) >= MinimumBlock)
            RBtree_delete(Prev(ptr));
        ptr = Prev(ptr);
    }
    Write(GetHead(ptr), size | 0x0);
    Write(GetFoot(ptr), size | 0x0);
    RBtree_insert(ptr);
    mm_checkblock(1, "", ptr);

    return ptr;
}

int checkNext(void *ptr, int size) {
    if (IsUsed(GetHead(Next(ptr)))) return 0;
    int tot = Length(GetHead(ptr)) + Length(GetHead(Next(ptr)));
    if (tot < size) return 0;
    RBtree_delete(Next(ptr));
    int last = tot - size;
    if (last < MinimumBlock) {
        Write(GetHead(ptr), tot | 0x1);
        Write(GetFoot(ptr), tot | 0x1);
    } else {
        Write(GetHead(ptr), size | 0x1);
        Write(GetFoot(ptr), size | 0x1);
        char *next = Next(ptr);
        Write(GetHead(next), last | 0x0);
        Write(GetFoot(next), last | 0x0);
        RBtree_insert(next);
    }
    return 1;
}

int cmp(Node a, Node b) {
    if (!b) return 1;
    if (!a) return -1;
    if (Length(GetHead(a)) > Length(GetHead(b))) return 1;
    if (Length(GetHead(a)) < Length(GetHead(b))) return -1;
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}

Node getNext(Node u) {
    if (RC(u)) {
        u = RC(u);
        while (u && LC(u))
            u = LC(u);
        return u;
    }
    Node fa = Parent(u), g = Parent(fa);
    while (g && u == RC(fa) && fa == RC(g)) {
        u = fa;
        fa = g;
        g = Parent(g);
    }
    return g;
}

void swap(Node *a, Node *b) {
    Node c;
    c = *a;
    *a = *b;
    *b = c;
}

void swapColor(Node a, Node b) {
    int c1 = Color(a), c2 = Color(b);
    SetColor(a, c2), SetColor(b, c1);
}

void swapNode(Node a, Node b) {
    swapColor(a, b);
    if (Parent(a) != b && Parent(b) != a) {
        if (LC(a)) Parent(LC(a)) = b;
        if (RC(a)) Parent(RC(a)) = b;
        if (LC(b)) Parent(LC(b)) = a;
        if (RC(b)) Parent(RC(b)) = a;
        if (Parent(a)) {
            if (a == LC(Parent(a)))
                LC(Parent(a)) = b;
            else
                RC(Parent(a)) = b;
        }
        if (Parent(b)) {
            if (b == LC(Parent(b)))
                LC(Parent(b)) = a;
            else
                RC(Parent(b)) = a;
        }
        swap(&LC(a), &LC(b));
        swap(&RC(a), &RC(b));
        swap(&Parent(a), &Parent(b));
    } else {
        int AIsChild = IsLChild(a), BIsLChild = IsLChild(b);
        if (LC(a) && LC(a) != b)
            Parent(LC(a)) = b;
        if (RC(a) && RC(a) != b)
            Parent(RC(a)) = b;
        if (LC(b) && LC(b) != a)
            Parent(LC(b)) = a;
        if (RC(b) && RC(b) != a)
            Parent(RC(b)) = a;
        if (Parent(a) && Parent(a) != b) {
            if (a == LC(Parent(a)))
                LC(Parent(a)) = b;
            else
                RC(Parent(a)) = b;
        }
        if (Parent(b) && Parent(b) != a) {
            if (b == LC(Parent(b)))
                LC(Parent(b)) = a;
            else
                RC(Parent(b)) = a;
        }
        if (Parent(a) == b) {
            Parent(a) = Parent(b);
            Parent(b) = a;
            if (AIsChild) {
                LC(b) = LC(a);
                LC(a) = b;
                swap(&RC(a), &RC(b));
            } else {
                RC(b) = RC(a);
                RC(a) = b;
                swap(&LC(a), &LC(b));
            }
        } else {
            Parent(b) = Parent(a);
            Parent(a) = b;
            if (BIsLChild) {
                LC(a) = LC(b);
                LC(b) = a;
                swap(&RC(a), &RC(b));
            } else {
                RC(a) = RC(b);
                RC(b) = a;
                swap(&LC(a), &LC(b));
            }
        }
    }
    Node *root = mem_heap_lo();
    if (!Parent(a)) *root = a;
    if (!Parent(b)) *root = b;
}

void takeOff(Node u) {
    LC(u) = RC(u) = 0;
    Node parent = Parent(u);
    if (parent) {
        if (u == RC(parent)) RC(parent) = 0;
        if (u == LC(parent)) LC(parent) = 0;
    }
    Parent(u) = 0;
}

void RBtree_insert(Address address) {
    Node *root = mem_heap_lo();
    Node cur = (Node) address;
    Node u = *root, parent = 0;
    while (u) {
        parent = u;
        if (cmp(u, cur) < 0) u = RC(u);
        else u = LC(u);
    }
    Parent(cur) = parent;
    LC(cur) = RC(cur) = 0;
    SetColor(cur, Red);
    if (!(*root)) *root = cur;
    if (parent) {
        if (cmp(parent, cur) > 0)
            LC(parent) = cur;
        else
            RC(parent) = cur;
    }
    solveDoubleRed(cur);
}

void RBtree_delete(Address address) {
    Node *root = mem_heap_lo();
    Node cur = (Node) address;
    Node next = 0, w = cur;
    if (!LC(cur)) next = RC(cur);
    else if (!RC(cur)) next = LC(cur);
    else {
        w = getNext(cur);
        if (w) {
            swapNode(w, cur);
            RBtree_delete((Address) cur);
        }
        return;
    }
    Node parent = Parent(cur);
    if (!parent) {
        if (LC(cur)) *root = LC(cur);
        else *root = RC(cur);
    }
    if (next) {
        Parent(next) = parent;
        if (parent) {
            if (cmp(parent, next) > 0)
                LC(parent) = next;
            else
                RC(parent) = next;
        }
    }
    takeOff(cur);

    if (!root)
        return;
    if (!parent) {
        SetColor(*root, Black);
        return;
    }
    if (!next) return;
    if (Color(next) == Red) {
        SetColor(next, Black);
        return;
    }
    solveDoubleBlack(next);
}

Node rotateAt(Node u) {
    Node p = Parent(u), g = Parent(p);
    if (IsLChild(p)) {
        if (IsLChild(u)) {
            Parent(p) = Parent(g);
            return connect34(u, p, g, LC(u), RC(u), RC(p), RC(g));
        } else {
            Parent(u) = Parent(g);
            return connect34(p, u, g, LC(p), LC(u), RC(u), RC(g));
        }
    } else {
        if (IsRChild(u)) {
            Parent(p) = Parent(g);
            return connect34(g, p, u, LC(g), LC(p), LC(u), RC(u));
        } else {
            Parent(u) = Parent(g);
            return connect34(g, u, p, LC(g), LC(u), RC(u), RC(p));
        }
    }
}

Node connect34(Node a, Node b, Node c, Node t0, Node t1, Node t2, Node t3) {
    LC(a) = t0;
    if (t0) Parent(t0) = a;
    RC(a) = t1;
    if (t1) Parent(t1) = a;
    LC(c) = t2;
    if (t2) Parent(t2) = c;
    RC(c) = t3;
    if (t3) Parent(t3) = c;
    LC(b) = a, Parent(a) = b;
    RC(b) = c, Parent(c) = b;
    return b;
}

void solveDoubleRed(Node x) {
    Node *root = mem_heap_lo();
    if (!Parent(x)) {
        SetColor(*root, Black);
        return;
    }
    Node p = Parent(x);
    if (Color(p) == Black) return;
    Node g = Parent(p);
    Node u = IsLChild(p) ? RC(g) : LC(g);
    if (Color(u) == Black) {
        if (IsLChild(x) == IsLChild(p))
            SetColor(p, Black);
        else
            SetColor(x, Black);
        SetColor(g, Red);
        Node gg = Parent(g);
        Node *tar = FromParentTo(g);
        *tar = rotateAt(x);
        Parent(*tar) = gg;
    } else {
        SetColor(p, Black);
        SetColor(u, Black);
        if (Parent(g))
            SetColor(g, Red);
        solveDoubleRed(g);
    }
}

void solveDoubleBlack(Node r) {
    Node *root = mem_heap_lo();
    Node p = Parent(r);
    if (!p) return;
    Node s = r == LC(p) ? LC(p) : RC(p);
    if (Color(s) == Black) {
        Node t = 0;
        if (Color(RC(s)) == Red) t = RC(s);
        if (Color(LC(s)) == Red) t = LC(s);
        if (t) {
            int oldColor = Color(p);
            Node *tar = FromParentTo(p);
            Node b = (*tar) = rotateAt(t);
            if (LC(b))
                SetColor(LC(b), Black);
            if (RC(b))
                SetColor(RC(b), Black);
            SetColor(b, oldColor);
        } else {
            SetColor(s, Red);
            if (Color(p) == Red)
                SetColor(p, Black);
            else
                solveDoubleBlack(p);
        }
    } else {
        SetColor(s, Black);
        SetColor(p, Red);
        Node t = IsLChild(s) ? LC(s) : RC(s);
        Node *tar = FromParentTo(p);
        (*tar) = rotateAt(t);
        solveDoubleBlack(r);
    }
}

int traverse(Node u) {
    int LHeight = 0, RHeight = 0;
    if (LC(u)) LHeight = traverse(LC(u));
    if (u) print(u);
    if (RC(u)) RHeight = traverse(RC(u));
    return ((LHeight > RHeight) ? LHeight : RHeight) + 1;
}

void print(Node u) {
    if (u) {
        printf("Address: %zx ", u);
        printf("Color: %d\n", Color(u));
        printf("Parent: %zx ", Parent(u));
        printf("lc: %zx ", LC(u));
        printf("rc: %zx ", RC(u));
        printf("Length: %d\n", Length(GetHead(u)));
    }
}

void mm_checkblock(int verbose, const char *func, void *bp) {
    if (!verbose) return;
    if (Read(GetHead(bp)) >> 3 != Read(GetFoot(bp)) >> 3) {
        printf("\n=========================== %s ===========================\n", func);
        printf("Error: %p's Header and footer are not match.\n", bp);
        printf("hsize = %d, fsize = %d\n", Length(GetHead(bp)), Length(GetFoot(bp)));
        printf("halloc = %d, falloc = %d\n", IsUsed(GetHead(bp)), IsUsed(GetFoot(bp)));
        printf("next_head_alloc = %d, next_footer_alloc = %d\n", IsUsed(GetHead(Next(bp))), IsUsed(GetFoot(Next(bp))));
        printf("=========================== %s ===========================\n", func);
    }
    if ((uint32) bp % Alignment != 0)
        printf("Error: %p's Payload area is not aligned.\n", bp);
    if (Length(GetHead(bp)) % Alignment != 0)
        printf("Error: %p payload size is not double word aligned.\n", bp);
}