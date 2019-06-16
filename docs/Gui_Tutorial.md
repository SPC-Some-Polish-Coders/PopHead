# GUI usage tutorial
-------------------------------------------------------


# Widget 
To understand how gui works first let's get to know `Widget` class. It represent every button or each </br>
other textured element on the screen. 

> So for example if creating a player equipment menu we can call every single item (sword, armor... etc.)</br>
by `Widget`.

There are usual methods so I believe everyone will understand how do they work but I would like to focus </br>
on unusual :

```cpp
// class Widget
...
virtual void addWidget(const std::string& name, Widget* ptr);

virtual void hide();

virtual void show();

virtual void moveAlongBranch(const sf::Vector2f& delta);

virtual void scaleAlongBranch(const sf::Vector2f& scale);

virtual void setAlphaAlongBranch(unsigned int alpha);

virtual void addBehavior(behaviorType type, const std::function<void(Widget*)>& func);

virtual Widget* getWidget(const std::string& name);

virtual bool isActive();

```

First look may discourage us but it only looks wild. Just imagine gui like tree - every single `widget` </br>
is a branch. When we take a look on branches we notice that some of they are divided into small branches.</br>
And that's the point! Now remembering the allusion I believe you will understand the idea:

+ addWidget
+ getWidget

Every Widget is a container for `widgets` and each `widget` is connected to other `Widget` or `Interface`.
>About Interface later...


When you call `addWidget` on `widget` **A** and pass `widget` **B** you create connection between `widgets` like </br>
parent - child. `getWidget` is useful when you want to get `child` `widget`. </br>

When the widget is drawn, it draws all children and then children of children... that's what I called</br>`branch`.

---

+ hide
+ show
+ isActive

This methods are useful when we want to stop drawing some `widgets`. For example we create `widgets` **A** </br>
 **B C**.</br>
**A** is `pause_menu` and others will be buttons. When player press `ESC` we call on **A** `show` method and now </br>
our `widgets` will be drawn. So if player will press `ESC` again we can `hide` **A** widget and **B**, **C** will be hide</br>
too because **A** is their `root` `widget`.

---

Now you should understand 
- moveAlongBranch
- scaleAlongBranch
- setAlphaAlongBranch

This method works for every children and children of chil... I mean work along `branch`.

---

I forgot about:
```cpp
void rePosition();
```

This method recalculating a position of `branch` I hope you will never be forced to use it.

---

> A few words about positioning : every widget are positioning by `root` widget. So if you set position</br>
> widget at sf::Vector2f(0.5f,0.5f) it means center of `root` widget.

# TextWidget

Class based on `Widget`. Posibility for add `Text`.

check : [Code](#Code)


# CRASH

Before you will use :

- Widget
  - addWidget
  - setContentPath
- TextWidget
  - setFontPath

make sure your `Widget` is connected to `Interface` or `Widget`

# Interface

`Interface` is a class which is drawn by GUI. It hasn't texture but organise `widgets` on the screen. So every</br>
`widgets` connected to `Interface` will be drawn. There are a lot of ways to using `Interface` you can create </br>
`Interface` for every gui window or even connect more than one widget to `Interface`.

Position of `Interface` is a point by which the `widgets` are oriented. By default set on center of view.


---

# GUI

It is base class which is drawing `Interfaces` so we can say every `branch` of `GUI`.


The most important methods :

```cpp
Widget* addInterface(const std::string& name);

Widget* getInterface(const std::string& name);
```

`addInterface` - it's the only way to create an `Interface`.</br>
`getInterface` - When you lost your pointer to your `Interface` you can get it by name </br>

---

If you know name of `Interface` you can do some actions using `GUI` class:  </br>


```cpp
void move(const sf::Vector2f&);

void deleteInterface(const std::string& name);

void showInterface(const std::string& name);

void hideInterface(const std::string& name);

void swap(const std::string& first, const std::string& second);

void moveUp(const std::string& name);

```

`move` - works for every `Interface`</br>
`deleteInterface` - be carefully </br>
`showInterface` and `hideInterface` - remind yourself Widget class </br>
`swap` and `moveUp` - the last `Interface` which added to `GUI` will be drawn as last so may you would like </br>
to change something.


---

# Code

If you still don't understand something check `GUI.hpp`

```cpp
/* Uncomment if you want to experiment   */

	//#define GUI_TEST
```

Then open `GUI.cpp`

```cpp

#ifdef GUI_TEST

	/* some wild examples */

	mGameData->getTextures().load("GuiTestContent/gui4.png");
	mGameData->getTextures().load("GuiTestContent/gui1.png");
	mGameData->getTextures().load("GuiTestContent/gui2.png");
	mGameData->getFonts().load("GuiTestContent/testFont.ttf");

	auto interface = mGameData->getGui().addInterface("game_pause_menu");

	auto mainWidget = new Widget;
	auto buttonFirstWidget = new TextWidget;
	auto buttonSecondWidget = new Widget;
//	auto mainSecondWidget = new Widget;



	//buttonSecondWidget->setOrigin(sf::Vector2f(0.5, 0.5));
	//buttonSecondWidget->setScale(sf::Vector2f(1, 1));
	//buttonSecondWidget->setPosition(sf::Vector2f(0.5, 0.7));

	//mainWidget->setPosition(sf::Vector2f(0.5, 0.5));
	interface->addWidget("buttonArea", mainWidget);
	mainWidget->setContentPath("GuiTestContent/gui4.png");
	mainWidget->setOrigin(sf::Vector2f(0.5, 0.5));
	//mainWidget->setPosition(sf::Vector2f(1, 1));
	//mainWidget->setScale(sf::Vector2f(3, 8));
	//interface->setPosition(sf::Vector2f(0.3, 0.3));
	//interface->addWidget("area2", mainSecondWidget);
	//mainWidget->addWidget("kek", mainSecondWidget);
	//mainSecondWidget->setContentPath("GuiTestContent/gui2.png");
	//mainSecondWidget->setOrigin(sf::Vector2f(0.5, 0.5));
	//mainSecondWidget->setPosition(sf::Vector2f(1, 1));

	mainWidget->addWidget("button1", buttonFirstWidget);
	buttonFirstWidget->setContentPath("GuiTestContent/gui1.png");
	buttonFirstWidget->setOrigin(sf::Vector2f(0.5f, 0.5f));
	buttonFirstWidget->setPosition(sf::Vector2f(0.5f, 0.3f));

	buttonFirstWidget->setFontPath("GuiTestContent/testFont.ttf");
	buttonFirstWidget->setTextPosition(sf::Vector2f(0.5f, 0.5f));
	buttonFirstWidget->setString("klik");
	buttonFirstWidget->setColor(sf::Color::Green);
	buttonFirstWidget->setTextPosition(sf::Vector2f(0.4f, 0.3f));

	buttonFirstWidget->addBehavior(behaviorType::onPressed, [](Widget* ptr)->void
	{
			auto widget = dynamic_cast<TextWidget*>(ptr);
			widget->setString("kliknieto");
			widget->setTextPosition(sf::Vector2f(0.3f, 0.3f));
	});
			
	mainWidget->addWidget("button2", buttonSecondWidget);
	buttonSecondWidget->setContentPath("GuiTestContent/gui2.png");
	buttonSecondWidget->setOrigin(sf::Vector2f(0.5f, 0.5f));
	buttonSecondWidget->setPosition(sf::Vector2f(0.5f, 0.7f));
		

	//interface->moveAlongBranch(sf::Vector2f(-50, -110));

			
#endif