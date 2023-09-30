#ifndef MLP_SRC_VIEW_VIEW_H_
#define MLP_SRC_VIEW_VIEW_H_
/** @cond */
#include <QMainWindow>
/** @endcond */

#include "../controller/controller.h"
#include "drawscene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow {
  Q_OBJECT

 public:
  View(QWidget *parent = nullptr);
  ~View();

 private:
  Ui::View *ui;
  s21::Controller ctr_;
  QString train_path_;
  QString test_path_;
  const QString kNoRecognizeg = "...";

 private slots:

  void OnSceneChanged();
  void on_open_img_btn_clicked();
  void on_save_img_btn_clicked();
  void on_crs_valid_btn_clicked();
  void on_upl_train_btn_clicked();
  void on_upl_test_btn_clicked();
  void on_train_btn_clicked();
  void on_test_btn_clicked();

  void on_upl_weights_btn_clicked();

  void on_save_weights_btn_clicked();

  void on_upd_network_btn_clicked();

 protected:
  void keyReleaseEvent(QKeyEvent *event) override;
};

#endif  // MLP_SRC_VIEW_VIEW_H_
