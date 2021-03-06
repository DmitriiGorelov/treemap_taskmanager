#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>
#include <QObject>
#include <QWidget>
#include <QMimeData>
#include <QFile>
#include <QFileInfo>
#include <QImageReader>

class MyTextEdit : public QTextEdit
{
        Q_OBJECT
    public:
        explicit MyTextEdit(QWidget *parent = nullptr);
        explicit MyTextEdit(const QString &text, QWidget *parent = nullptr);

        bool canInsertFromMimeData(const QMimeData* source) const final
        {
            return source->hasImage() || source->hasUrls() ||
                QTextEdit::canInsertFromMimeData(source);
        }

        void insertFromMimeData(const QMimeData* source) final
        {
            if (source->hasImage())
            {
                static int i = 1;
                QUrl url(QString("dropped_image_%1").arg(i++));
                dropImage(url, qvariant_cast<QImage>(source->imageData()));
            }
            else if (source->hasUrls())
            {
                foreach (QUrl url, source->urls())
                {
                    QFileInfo info(url.toLocalFile());
                    if (QImageReader::supportedImageFormats().contains(info.suffix().toLower().toLatin1()))
                        dropImage(url, QImage(info.filePath()));
                    else
                        dropTextFile(url);
                }
            }
            else
            {
                QTextEdit::insertFromMimeData(source);
            }
        }

        private:
            void dropImage(const QUrl& url, const QImage& image)
            {
                if (!image.isNull())
                {
                    document()->addResource(QTextDocument::ImageResource, url, image);
                    textCursor().insertImage(url.toString());
                }
            }

            void dropTextFile(const QUrl& url)
            {
                QFile file(url.toLocalFile());
                if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                    textCursor().insertText(file.readAll());
            }
};

#endif // MYTEXTEDIT_H
