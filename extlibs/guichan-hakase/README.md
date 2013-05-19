guichan-hakase
==============

Additional widgets and utilities for [Guichan](http://gitorious.org/guichan). 

## Fonts ##

### FixedImageFont ###
Similar to `gcn::ImageFont` except that all glyphs are expected to be the same 
size. The image and glyph sizes are set at construction time.

```c++
const std::string fontImageFileName = "gui-font.png";
const std::string fontGlyphs = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const int glyphSize = 8;

std::unique_ptr<gcn::Image> fontImage(gcn::Image::load(fontImageFileName));

std::unique_ptr<gcn::FixedImageFont> fixedImageFont(
        new gcn::FixedImageFont(fontImage.get(), glyphSize, fontGlyphs));

//
// Now you can draw your image font all over everything
//
gcn::Widget::setGlobalFont(fixedImageFont.get());
```

## Utilities ##

### FunctorActionListener ###
An adapter that glues together `std::function` and `gcn::ActionListener` which 
allows the use of arbitrary functions as callbacks to handle `ActionEvent`s 
without having to implement the `gcn::ActionListener` interface. Favors 
composition over inheritance.

```c++
//
// Create a button first
//
std::unique_ptr<gcn::Button> startButton(new gcn::Button("Start"));

//
// Now you can use lambdas to handle events...
//
gcn::FunctorActionListener lambdaButtonListener([](const gcn::ActionEvent& actionEvent) {
    std::cout << "Button clicked! " << actionEvent.getId() << std::endl;
});

//
// ...or you can use free functions to handle events
//
void handleButtonClick(const gcn::ActionEvent& actionEvent) {
    std::cout << "handleButtonClick [" << actionEvent.getId() << "]" << std::endl;
}

gcn::FunctorActionListener freeFunctionButtonListener();
freeFunctionButtonListener->setCallback(gcn::ActionListenerCallback(handleButtonClick));

startButton->addActionListener(&lambdaButtonListener);
startButton->addActionListener(&freeFunctionButtonListener);
```