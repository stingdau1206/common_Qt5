#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QMutex>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void setImage(const QImage &image); // Set the QR code for this provider

private:
    QImage m_image; // Stores the QR code image
    QMutex m_mutex;
};
#endif // IMAGEPROVIDER_H
