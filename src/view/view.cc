#include "view.h"
/** @cond */
#include <QFileDialog>
#include <QMessageBox>

#include "./ui_view.h"
/** @endcond */

View::View(QWidget *parent) : QMainWindow(parent), ui(new Ui::View) {
  ui->setupUi(this);
  connect(ui->draw_scene, &DrawScene::SceneChanged, this,
          &View::OnSceneChanged);
  ui->report_graph->addGraph();
  QCPScatterStyle sstyle(QCPScatterStyle::ssCircle);
  ui->report_graph->graph()->setScatterStyle(sstyle);
  ui->report_graph->xAxis->setRange(1, 10);
  ui->report_graph->yAxis->setRange(1, 105);
  //  QVector<double> y{10,20,30,40,50,60,70,80,90,100};
  //  QVector<double>x{1,2,3,4,5,6,7,8,9,10};
  //  ui->report_graph->graph()->setData(x,y);
  ui->report_graph->replot();
  //  ui->report_graph->yAxis->grid()
}

View::~View() { delete ui; }

void View::OnSceneChanged() {
  qDebug() << "got signal";
  if (!ctr_.IsNull()) {
    ctr_.Recognize(ui->draw_scene->GetVector());
    char recogn = (char)((int)'A' + ctr_.Recognized().first);
    ui->recognized_char_lb->setText(QString(recogn));
    qDebug() << ctr_.Recognized();
  }
}

void View::on_upl_train_btn_clicked() {
  QString path = QFileDialog::getOpenFileName(this, "Open train sample");
  if (!path.isEmpty()) {
    ctr_.ReadTrainDataset(path.toStdString());
    train_path_ = path;
    ui->report_tb->clear();
    ui->report_tb->append("Train dataset: " + train_path_);
    ui->report_tb->append(
        "Size: " + QString::number(ctr_.TrainSample().size()) + "\n");
    if (!ctr_.TestSample().empty()) {
      ui->report_tb->append("Test dataset: " + test_path_);
      ui->report_tb->append(
          "Size: " + QString::number(ctr_.TestSample().size()) + "\n");
    }
  }
}

void View::on_upl_test_btn_clicked() {
  QString path = QFileDialog::getOpenFileName(this, "Open tests sample");
  if (!path.isEmpty()) {
    ctr_.ReadTestDataset(path.toStdString());
    test_path_ = path;
    ui->report_tb->clear();
    if (!ctr_.TrainSample().empty()) {
      ui->report_tb->append("Train dataset: " + train_path_);
      ui->report_tb->append(
          "Size: " + QString::number(ctr_.TrainSample().size()) + "\n");
    }
    ui->report_tb->append("Test dataset: " + test_path_);
    ui->report_tb->append("Size: " + QString::number(ctr_.TestSample().size()) +
                          "\n");
    setEnabled(true);
  }
}

void View::on_train_btn_clicked() {
  qDebug() << "train";
  if (ctr_.TrainSample().empty() or ctr_.TestSample().empty()) {
    QMessageBox::about(this, "", "Open samples first");
    return;
  }
  if (ui->gen_rep_ch->isChecked()) {
    auto rep = ctr_.TrainWithReport(ui->epoches_sb->value(),
                                    ui->sample_size_sb->value());
    QVector<double> x, y;
    for (int i = 0; i < rep.size(); ++i) {
      x.push_back((double)(i + 1));
      y.push_back(rep[i]);
    }
    ui->report_graph->xAxis->setRange(1, (int)x.size());
    ui->report_graph->graph()->setData(x, y);
    ui->report_graph->replot();

  } else {
    ctr_.Train(ui->epoches_sb->value());
  }
}

void View::on_crs_valid_btn_clicked() {
  qDebug() << "cross validation";
  if (ctr_.TrainSample().empty()) {
    QMessageBox::about(this, "", "Open samples first");
    return;
  }

  auto rep =
      ctr_.CrossValidation(ui->parts_cv_sb->value(), ui->epoches_sb->value());

  if (ui->gen_rep_ch->isChecked()) {
    QVector<double> x, y;
    for (int i = 0; i < rep.size(); ++i) {
      x.push_back((double)(i + 1));
      y.push_back(rep[i]);
    }
    ui->report_graph->xAxis->setRange(1, (int)x.size());
    ui->report_graph->graph()->setData(x, y);
    ui->report_graph->replot();
  }
}

void View::on_test_btn_clicked() {
  qDebug() << "tests";
  if (ctr_.TestSample().empty()) {
    QMessageBox::about(this, "", "Open tests sample first");
  } else {
    auto res = ctr_.Test(ui->sample_size_sb->value());
    ui->report_tb->clear();
    ui->report_tb->append("Average Accuracy = " +
                          QString::number(res.avg_accuracy, 'g', 2));
    ui->report_tb->append("Precision = " + QString::number(res.precision));
    ui->report_tb->append("Recall = " + QString::number(res.recall));
    ui->report_tb->append("F-measure = " + QString::number(res.f_measure));
    ui->report_tb->append(
        "Test sample size = " +
        QString::number(int(ui->sample_size_sb->value() *
                            (double)ctr_.TestSample().size())));
    ui->report_tb->append("Testing time = " + QString::number(res.seconds) +
                          " seconds");
  }
}

void View::on_open_img_btn_clicked() {
  QString file_path = QFileDialog::getOpenFileName(
      this, QFileDialog::tr("Open file"), QFileDialog::tr("(*.bmp)"));
  if (!file_path.isEmpty()) {
    ui->draw_scene->SetImage(file_path);
  }
}

void View::on_save_img_btn_clicked() {
  char recognized;
  QString file_path;
  if (!ctr_.IsNull()) {
    recognized = (char)('A' + ctr_.Recognized().first);
    file_path =
        QFileDialog::getSaveFileName(this, "Save Image", QString(recognized));
  } else
    file_path = QFileDialog::getSaveFileName(this, "Save Image");
  if (!file_path.isEmpty())
    ui->draw_scene->GetImage().save(file_path + ".bmp", "BMP");
}

void View::keyReleaseEvent(QKeyEvent *event) {
  QWidget::keyReleaseEvent(event);
  qDebug() << event;
}

void View::on_upl_weights_btn_clicked() {
  auto path =
      QFileDialog::getOpenFileName(this, "Open weights", "", "(*.weights)");
  if (!path.isEmpty()) ctr_.ImportWeights(path.toStdString());
}

void View::on_save_weights_btn_clicked() {
  auto path = QFileDialog::getSaveFileName(this, "Save weights") + ".weights";
  if (!path.isEmpty()) ctr_.ExportWeights(path.toStdString());
}

void View::on_upd_network_btn_clicked() {
  ui->recognized_char_lb->setText(kNoRecognizeg);
  s21::NetworkType network_type = (ui->matrix_type_rb->isChecked())
                                      ? s21::NetworkType::MATRIX
                                      : s21::NetworkType::GRAPH;
  ui->report_graph->graph()->data().reset();
  ctr_.UpdateNetwork(ui->h_layers_sb->value(), network_type);
  ui->report_tb->clear();
  ui->report_tb->append((ctr_.GetType() == s21::NetworkType::MATRIX)
                            ? "Matrix network"
                            : "Graph network");
  ui->report_tb->append(QString::number(ctr_.GetSize()) + " layers");
  ui->report_tb->append(QString::number(ctr_.GetSize() - 2) + " hidden layers");
}
