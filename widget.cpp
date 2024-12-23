#include "widget.h"
#include "ui_widget.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>

Widget::Widget(QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::Widget)
{
  ui->setupUi(this);
  listWgt = findChild<QListWidget*>("taskList");
  addTaskBtn = findChild<QPushButton*>("addTaskBtn");
  remTaskBtn = findChild<QPushButton*>("remTaskBtn");

  connect(addTaskBtn, SIGNAL(clicked()), this, SLOT(addTaskItem()));
  connect(remTaskBtn, SIGNAL(clicked()), this, SLOT(remTaskItem()));
  connect(listWgt, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(checkTaskItem(QListWidgetItem*)));

  QSettings settings(QSettings::IniFormat, QSettings::UserScope, "NA", "taskList", this);

  settings.beginGroup("GUI");
  int wdwSizeX = settings.value("sizeX", 320).toInt();
  int wdwSizeY = settings.value("sizeY", 240).toInt();
  settings.endGroup();

  this->resize(wdwSizeX, wdwSizeY);
}

Widget::~Widget()
{
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, "NA", "taskList", this);

  settings.beginGroup("GUI");
  settings.setValue("sizeX", this->width());
  settings.setValue("sizeY", this->height());
  settings.endGroup();
  settings.sync();

  delete ui;
}

void Widget::addTaskItem()
{
  // call dialog
  bool ok;
  QString taskText = QInputDialog::getText(this, tr("Input task text"), tr("Task text:"), QLineEdit::Normal, QString(), &ok);
  if (ok && taskText.isEmpty()) {
	QMessageBox* box = new QMessageBox(QMessageBox::Warning, "", tr("No text entered!"), QMessageBox::Ok, this);
	box->exec();
	box->deleteLater();
	return;
  }
  else if (!ok) {
	return;
  }

  // add new item to list
  QListWidgetItem* item = new QListWidgetItem(taskText, listWgt);
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren);
  item->setCheckState(Qt::Unchecked);
}

void Widget::remTaskItem()
{
  if (listWgt->count() > 0) {
	for (QListWidgetItem* item : listWgt->selectedItems()) {
	  listWgt->removeItemWidget(item);
	  delete item;
	}
  }
}

void Widget::checkTaskItem(QListWidgetItem* item)
{
  QFont font;
  switch (item->checkState())
  {
	case Qt::Checked:
	  font = item->font();
	  font.setStrikeOut(true);
	  item->setFont(font);
	  break;
	case Qt::Unchecked:
	  font = item->font();
	  font.setStrikeOut(false);
	  item->setFont(font);
	  break;
	default:
	  break;
  }
}
