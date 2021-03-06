#ifndef _PAGE_H_
#define _PAGE_H_

#include <atomic>
#include <gtkmm.h>

#include "curler.h"
#include "site.h"
#include "imagelist.h"
#include "xml.h"

namespace AhoViewer
{
    namespace Booru
    {
        class Image;
        class Page : public Gtk::ScrolledWindow,
                     public ImageList::Widget
        {
            friend class Browser;

            using SignalClosedType = sigc::signal<void, Page*>;
            using SignalDownloadErrorType = sigc::signal<void, const std::string>;
            using SignalSaveProgressType = sigc::signal<void, const Page*>;
        public:
            Page();
            virtual ~Page() override;

            std::shared_ptr<Site> get_site() const { return m_Site; }
            std::shared_ptr<ImageList> get_imagelist() const { return m_ImageList; }
            size_t get_page_num() const { return m_Page; }

            SignalClosedType signal_closed() const { return m_SignalClosed; }
            SignalDownloadErrorType signal_no_results() const { return m_SignalDownloadError; }
            SignalSaveProgressType signal_save_progress() const { return m_SignalSaveProgress; }
        protected:
            virtual void set_pixbuf(const size_t index, const Glib::RefPtr<Gdk::Pixbuf> &pixbuf) override;
            virtual void set_selected(const size_t index) override;
            virtual void scroll_to_selected() override;
        private:
            void search(const std::shared_ptr<Site> &site);
            void save_image(const std::string &path, const std::shared_ptr<Image> &img);
            void save_images(const std::string &path);
            bool ask_cancel_save();

            void cancel_save();
            void get_posts();
            bool get_next_page();

            Gtk::Widget* get_tab() const { return m_Tab; }
            bool is_saving() const { return m_Saving; }

            void on_posts_downloaded();
            void on_selection_changed();
            void on_value_changed();
            virtual bool on_button_press_event(GdkEventButton *e) override;
            bool on_tab_button_release_event(GdkEventButton *e);

            Gtk::Menu *m_PopupMenu;
            Gtk::IconView *m_IconView;
            Gtk::EventBox *m_Tab;
            Gtk::Image *m_TabIcon;
            Gtk::Label *m_TabLabel;
            Gtk::Button *m_TabButton;

            std::shared_ptr<ImageList> m_ImageList;
            std::shared_ptr<Site> m_Site;
            Curler m_Curler;

            // m_Tags stores tags that are inside the entry
            // while m_SearchTags are the whitespace trimmed tags or *
            std::string m_Tags, m_SearchTags, m_Path;
            size_t m_Page,
                   m_SaveImagesTotal;
            std::atomic<size_t> m_SaveImagesCurrent;
            std::atomic<bool> m_Saving;
            bool m_LastPage, m_KeepAligned;
            std::unique_ptr<xml::Document> m_Posts;

            Glib::RefPtr<Gio::Cancellable> m_SaveCancel;
            std::thread m_GetPostsThread,
                        m_SaveImagesThread;
            Glib::Dispatcher m_SignalPostsDownloaded,
                             m_SignalSaveProgressDisp;

            sigc::connection m_GetNextPageConn,
                             m_ScrollConn;

            SignalClosedType m_SignalClosed;
            SignalDownloadErrorType m_SignalDownloadError;
            SignalSaveProgressType m_SignalSaveProgress;
        };
    }
}

#endif /* _PAGE_H_ */
