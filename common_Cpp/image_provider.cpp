#include "image_provider.h"

ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{
    m_image = QImage();
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    QMutexLocker locker(&m_mutex);  // Thread-safe

    if (size) {
        *size = m_image.size();
    }
    if (requestedSize.isValid()) {
        return m_image.scaled(requestedSize);
    } else {
        return m_image;
    }
}

void ImageProvider::setImage(const QImage &image)
{
    QMutexLocker locker(&m_mutex);  // Thread-safe
    if(m_image != image)
    {
        m_image = image;
    }
}
