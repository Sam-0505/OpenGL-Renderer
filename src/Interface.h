#ifndef INTERFACE_H
#define INTERFACE_H

class Interface
{
public:
	Interface();
	~Interface();

	bool initImGui();
	void show();
	bool show_demo_window;
	bool show_another_window;

};

#endif // INTERFACE_H
