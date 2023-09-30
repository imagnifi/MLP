#include "drawscene.h"
/** @cond */
#include <QDebug>
/** @endcond */

DrawScene::DrawScene(QWidget *parent)
    : QWidget(parent),
      image_(QSize(kMaxImgSize, kMaxImgSize), QImage::Format_Grayscale16) {
  ClearImage();
}

void DrawScene::SetImage(const QString &file_path) {
  ClearImage();
  image_.load(file_path);
  image_ = image_.scaled(kMaxImgSize, kMaxImgSize)
               .convertToFormat(QImage::Format_Grayscale16);
  update();
}

const QImage &DrawScene::GetImage() { return image_; }

void DrawScene::ClearImage() {
  image_.fill(QColor(255, 255, 255));
  update();
}

void DrawScene::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  QRect rect(event->rect());
  painter.drawImage(rect, image_, rect);
}

void DrawScene::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton or event->button() == Qt::RightButton) {
    begin_line_ = event->pos();
  }
}

void DrawScene::mouseReleaseEvent(QMouseEvent *event) {
  QWidget::mouseReleaseEvent(event);
  if (event->button() == Qt::LeftButton or event->button() == Qt::RightButton) {
    emit SceneChanged();
  }
  qDebug() << event;
}

void DrawScene::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    QPainter painter(&image_);
    painter.setPen(QPen(Qt::black, kBrushWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    QPoint newPoint = event->pos();
    painter.drawLine(begin_line_.x(), begin_line_.y(), newPoint.x(),
                     newPoint.y());
    begin_line_ = newPoint;
    update();
  } else if (event->buttons() == Qt::RightButton) {
    QPainter painter(&image_);
    painter.setPen(QPen(Qt::white, kBrushWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    QPoint newPoint = event->pos();
    painter.drawLine(begin_line_.x(), begin_line_.y(), newPoint.x(),
                     newPoint.y());
    begin_line_ = newPoint;
    update();
  }
}

void DrawScene::mouseDoubleClickEvent(QMouseEvent *event) {
  QWidget::mouseDoubleClickEvent(event);
  if (event->button() == Qt::RightButton) ClearImage();
}

auto DrawScene::GetVector() -> std::vector<double> {
  std::vector<double> input_layer;
  input_layer.reserve(kMinImgSize * kMinImgSize);
  auto scaled_img = image_.scaled(kMinImgSize, kMinImgSize, Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation);
  for (int i = 0; i < kMinImgSize; ++i)
    for (int j = 0; j < kMinImgSize; ++j)
      input_layer.push_back((double)scaled_img.pixelColor(i, j).blackF());


  return input_layer;
}
