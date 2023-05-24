#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QKeyEvent>
#include <QTransform>

class ImageViewer : public QLabel
{
public:
    ImageViewer(QWidget *parent = nullptr)
        : QLabel(parent), scaleFactor(1.0)
    {
        setWindowTitle("Image Viewer");
    }

    void loadImage(const QString &imagePath)
    {
        QPixmap image(imagePath);
        if (image.isNull()) {
            setText("Failed to load image.");
        } else {
            setPixmap(image);
            setScaledContents(true);
            resize(image.size());
            scaleFactor = 1.0;
        }
    }

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        switch (event->key()) {
        case Qt::Key_Plus:
            zoomIn();
            break;
        case Qt::Key_Minus:
            zoomOut();
            break;
        case Qt::Key_Left:
            rotateLeft();
            break;
        case Qt::Key_Right:
            rotateRight();
            break;
        case Qt::Key_R:
            flip();
            break;
        default:
            QLabel::keyPressEvent(event);
            break;
        }
    }

private:
    void zoomIn()
    {
        scaleImage(1.25);
    }

    void zoomOut()
    {
        scaleImage(0.8);
    }

    void rotateLeft()
    {
        const QPixmap* currentPixmap = pixmap();
        if (currentPixmap && !currentPixmap->isNull()) {
            QPixmap rotatedPixmap = currentPixmap->transformed(QTransform().rotate(-90));
            setPixmap(rotatedPixmap);
        }
    }

    void rotateRight()
    {
        const QPixmap* currentPixmap = pixmap();
        if (currentPixmap && !currentPixmap->isNull()) {
            QPixmap rotatedPixmap = currentPixmap->transformed(QTransform().rotate(90));
            setPixmap(rotatedPixmap);
        }
    }

    void flip()
    {
        const QPixmap* currentPixmap = pixmap();
        if (currentPixmap && !currentPixmap->isNull()) {
            QPixmap flippedPixmap = currentPixmap->transformed(QTransform().scale(-1, 1));
            setPixmap(flippedPixmap);
        }
    }

    void scaleImage(float factor)
    {
        scaleFactor *= factor;
        setScaledContents(true);
        resize(scaleFactor * sizeHint());
    }

    qreal scaleFactor;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ImageViewer viewer;
    if (argc > 1) {
        viewer.loadImage(argv[1]);
    } else {
        viewer.setText("No image specified.");
    }

    viewer.show();
	return app.exec();
}

