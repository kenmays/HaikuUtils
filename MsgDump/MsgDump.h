#ifndef _MSGDUMP_H_
#define _MSGDUMP_H_

#include <Application.h>
#include <Window.h>
#include <View.h>
#include <String.h>
#include <ObjectList.h>

class TestWindow;
class BTextControl;
class BColumnListView;
class BRow;
class BEntry;

struct Operation
{
	enum Kind {
		insertOp,
		removeOp,
		updateOp
	};
	BString path;
	Kind kind;
	int8 *data;
	ssize_t size;
	BMessage msg;
	
	Operation() {}
	Operation(BString &path, Kind kind, const int8 *data, ssize_t size): path(path), kind(kind), size(size) {
		if (size <= 0) {
			this->data == NULL;
		} else {
			this->data = new int8[size];
			memcpy(this->data, data, size);
		}
	}
	~Operation() {if (data != NULL) delete[] data;}
};

struct MessageLoc
{
	BMessage &msg;
	BString name;
	int32 idx;
	int32 ofs;
	bool useOfs;
};

class EditWindow: public BWindow
{
private:
	TestWindow *base;
	BView *rootView;
	BTextControl *nameView, *typeView, *valueView;
	
public:
	EditWindow(TestWindow *base, BRect frame);
	void SetTo(BRow *row);
	void Quit();
};

class TestWindow: public BWindow
{
private:
	BView *rootView;
	BMenuBar *menu;
	BTextControl *pathView;
	BColumnListView *view;
	EditWindow *editWnd;
	BMessage message;
	
	BObjectList<Operation> undoStack, redoStack;
	
	friend class EditWindow;
	
public:
	TestWindow(BRect frame);
	bool Load(BEntry &entry);
	void Quit();
	void MessageReceived(BMessage* msg);
};

class TestApplication: public BApplication
{
public:
	TestApplication();
	void RefsReceived(BMessage *refsMsg);
	void ReadyToRun();
};

#endif	// _MSGDUMP_H_
