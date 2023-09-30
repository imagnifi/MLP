#ifndef DRAWSCENE_H
#define DRAWSCENE_H
/** @cond */
#include <QColor>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <vector>
/** @endcond */

class DrawScene : public QWidget {
  Q_OBJECT

 public:
  static constexpr int kMinImgSize = 28;
  static constexpr int kMaxImgSize = 512;
  explicit DrawScene(QWidget *parent = nullptr);
  ~DrawScene() override = default;

  auto SetImage(const QString &file_path) -> void;
  auto GetImage() -> const QImage &;
  auto ClearImage() -> void;
  auto GetVector() -> std::vector<double>;

 signals:
  void SceneChanged();

 protected:
  auto paintEvent(QPaintEvent *event) -> void override;
  auto mousePressEvent(QMouseEvent *event) -> void override;
  auto mouseMoveEvent(QMouseEvent *event) -> void override;
  auto mouseDoubleClickEvent(QMouseEvent *event) -> void override;
  void mouseReleaseEvent(QMouseEvent *event) override;

 private:
  const int kBrushWidth = 50;
  QImage image_;
  QPoint begin_line_;
};

#endif  // DRAWSCENE_H
