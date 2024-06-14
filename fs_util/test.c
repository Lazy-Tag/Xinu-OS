extern void uprintf(void);
extern void usleepms(uint32);

int main() {
	uprintf();
	usleepms(2000);
	return 0;
}

