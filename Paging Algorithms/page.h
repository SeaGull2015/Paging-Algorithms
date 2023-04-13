#pragma once
class page
{
	int id;
	int added;
	int lastCalled;
	int uses;
public:
	page(int ID) {
		id = ID;
		added = 0;
		lastCalled = 0;
		uses = 0;
	}

	page() { // ghost page
		id = -1;
		added = 0;
		lastCalled = 0;
	}

	int getAdded() {
		return added;
	}

	int getLastCalled() {
		return lastCalled;
	}

	int getId() {
		return id;
	}

	int getUses() {
		return uses;
	}

	void call(int t = 0) {
		lastCalled = t;
		uses++;
	};
};

