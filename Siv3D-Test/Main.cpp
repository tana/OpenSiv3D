
# include <Siv3D.hpp> // OpenSiv3D v0.2.3

class MainClass {
private:
    const Font font;

    const Texture textureCat;

public:
    MainClass()
      : font(50)
      , textureCat(Emoji(U"🐈"), TextureDesc::Mipped)
    {
        Logger << U"Hello, World!";
        Graphics::SetBackground(ColorF(0.8, 0.9, 1.0));
    }

    bool Update()
    {
        if (!System::Update())
        {
            return false;
        }

        // change background color based on cursor position
        int x = Cursor::Pos().x;
        int y = Cursor::Pos().y;
        Graphics::SetBackground(ColorF((float)x / 640.0, (float)y / 480.0, 0.0));

        font(U"Hello, Siv3D!🐣").drawAt(Window::Center(), Palette::Black);

        font(Cursor::Pos()).draw(20, 400, ColorF(0.6));

        textureCat.resized(80).draw(540, 380);

        Circle(Cursor::Pos(), 60).draw(ColorF(1, 0, 0, 0.5));

        return true;
    }

    ~MainClass()
    {
        Logger << U"Goodbye";
    }
};

MainClass* mainClass;
void OnStart()
{
    mainClass = new MainClass();
}

bool OnUpdate()
{
    return mainClass->Update();
}

void OnExit()
{
    delete mainClass;
}
