#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <giomm/application.h>

namespace AhoViewer
{
    class MainWindow;
    class Application : public Gio::Application
    {
    public:
        static Application& get_instance();

        MainWindow* create_window();

        int run(int argc, char **argv);
    protected:
        Application();

        virtual void on_open(const std::vector<Glib::RefPtr<Gio::File>> &f,
                             const Glib::ustring&) override;
        virtual void on_startup() override;
        virtual void on_activate() override;
    private:
        void add_window(MainWindow* w);
        void remove_window(MainWindow* w);

        std::vector<MainWindow*> m_Windows;
    };
}

#endif /* _APPLICATION_H_ */
