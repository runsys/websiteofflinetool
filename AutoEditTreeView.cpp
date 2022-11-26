#include "AutoEditTreeView.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include "../../UtilityFunc/qtoolfunc.h"


AutoEditTreeViewDelegate::AutoEditTreeViewDelegate(QObject* parent)
	:QItemDelegate(parent)
{
	connect(this, SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)), this, SLOT(MycloseEditor(QWidget *, QAbstractItemDelegate::EndEditHint)));
}

void AutoEditTreeViewDelegate::MycloseEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint)
{
	emit EditorClosed();
}
QWidget* AutoEditTreeViewDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	QLineEdit* editor = new QLineEdit(parent);
	return editor;
}

void AutoEditTreeViewDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	if (index.isValid())
	{
		QLineEdit* lineE = qobject_cast<QLineEdit*>(editor);
		QString showdata = index.data(Qt::DisplayRole).toString();
		auto itemdata = index.data(Qt::DisplayRole).toString()+"\n"+index.data(257).toString();
		lineE->setText(showdata);//text() == data(Qt::DispalyRole).toString()
	}
}

void AutoEditTreeViewDelegate::setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex& index) const
{
	if (index.isValid())
	{
		QLineEdit* lineE = qobject_cast<QLineEdit*>(editor);
		auto str = lineE->text();
		QString namestr = str;
		if (str.indexOf('\n') != -1)
		{
			namestr = str.mid(0, str.indexOf('\n'));
			str = str.mid(str.indexOf('\n') + 1);
		}
		else{
			str = "";
		}
		model->setData(index, namestr);

		QMap<int, QVariant> role_data = model->itemData(index);
		if (role_data.find(257) == role_data.end())
		{
			role_data.insert(257, str);
		}
		else{
			role_data[257] = str;
		}

		model->setItemData(index, role_data);
	}
}

void AutoEditTreeViewDelegate::updateEditorGeometry(QWidget* editor,
	const QStyleOptionViewItem& option,
	const QModelIndex& index
	) const
{
	((QLineEdit*)editor)->setGeometry(option.rect);
}


bool AutoEditTreeViewDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	return QItemDelegate::editorEvent(event, model, option, index);
}

AutoEditTreeView::AutoEditTreeView(QWidget *parent) :
    QTreeView(parent)
{
	mmodel = new QStandardItemModel;
	mmodel->setHorizontalHeaderItem(0, new QStandardItem(wtr(L"网址")));
	mmodel->setHorizontalHeaderItem(1, new QStandardItem(wtr(L"本地目录")));
	mmodel->setHorizontalHeaderItem(2, new QStandardItem(wtr(L"包含下载路径")));
	mmodel->setHorizontalHeaderItem(3, new QStandardItem(wtr(L"排除下载路径")));
	mmodel->setHorizontalHeaderItem(4, new QStandardItem(wtr(L"线程数")));
	mmodel->setHorizontalHeaderItem(5, new QStandardItem(wtr(L"状态")));
	mmodel->setHorizontalHeaderItem(6, new QStandardItem(wtr(L"等待秒")));
	mmodel->setHorizontalHeaderItem(7, new QStandardItem(wtr(L"连续下载秒")));
	mmodel->setHorizontalHeaderItem(8, new QStandardItem(wtr(L"更新链接匹配")));
	mmodel->setHorizontalHeaderItem(9, new QStandardItem(wtr(L"更新链接秒")));
	mmodel->setHorizontalHeaderItem(10, new QStandardItem(wtr(L"下链接间隔毫秒")));
	mmodel->setHorizontalHeaderItem(11, new QStandardItem(wtr(L"UserAgent")));
	mmodel->setHorizontalHeaderItem(12, new QStandardItem(wtr(L"登录链接")));
	mmodel->setHorizontalHeaderItem(13, new QStandardItem(wtr(L"登录post数据")));
	mmodel->setHorizontalHeaderItem(14, new QStandardItem(wtr(L"登录成功匹配")));
	mmodel->setHorizontalHeaderItem(15, new QStandardItem(wtr(L"URL匹配后缀")));
	mmodel->setHorizontalHeaderItem(16, new QStandardItem(wtr(L"重链接到本地")));
	mmodel->setHorizontalHeaderItem(17, new QStandardItem(wtr(L"文件存在不下载URL匹配")));
	mmodel->setHorizontalHeaderItem(18, new QStandardItem(wtr(L"存储到rockdb")));
	mmodel->setHorizontalHeaderItem(19, new QStandardItem(wtr(L"不存在头匹配")));
	mmodel->setHorizontalHeaderItem(20, new QStandardItem(wtr(L"不存在页匹配")));
	mmodel->setHorizontalHeaderItem(21, new QStandardItem(wtr(L"页面超时")));

	mmodel->setHorizontalHeaderItem(22, new QStandardItem(wtr(L"URL提取正则表达式1")));
	mmodel->setHorizontalHeaderItem(23, new QStandardItem(wtr(L"组合URL正则替换式1")));
	mmodel->setHorizontalHeaderItem(24, new QStandardItem(wtr(L"组合URL时间替换式1")));
	mmodel->setHorizontalHeaderItem(25, new QStandardItem(wtr(L"组合URL标题替换式1")));
	mmodel->setHorizontalHeaderItem(26, new QStandardItem(wtr(L"提取后任务状态值1")));

	mmodel->setHorizontalHeaderItem(27, new QStandardItem(wtr(L"URL提取正则表达式2")));
	mmodel->setHorizontalHeaderItem(28, new QStandardItem(wtr(L"组合URL正则替换式2")));
	mmodel->setHorizontalHeaderItem(29, new QStandardItem(wtr(L"组合URL时间替换式2")));
	mmodel->setHorizontalHeaderItem(30, new QStandardItem(wtr(L"组合URL标题替换式2")));
	mmodel->setHorizontalHeaderItem(31, new QStandardItem(wtr(L"提取后任务状态值2")));

	mmodel->setHorizontalHeaderItem(32, new QStandardItem(wtr(L"PageGate服务器列表")));
	mmodel->setHorizontalHeaderItem(33, new QStandardItem(wtr(L"PageGate抓URL匹配1")));
	mmodel->setHorizontalHeaderItem(34, new QStandardItem(wtr(L"PageGateScript1")));
	mmodel->setHorizontalHeaderItem(35, new QStandardItem(wtr(L"PageGate抓URL匹配2")));
	mmodel->setHorizontalHeaderItem(36, new QStandardItem(wtr(L"PageGateScript2")));
	mmodel->setHorizontalHeaderItem(37, new QStandardItem(wtr(L"ContentDisposition排除匹配")));
	mmodel->setHorizontalHeaderItem(38, new QStandardItem(wtr(L"页面完整URL匹配1")));
	mmodel->setHorizontalHeaderItem(39, new QStandardItem(wtr(L"页面完整正则1")));
	mmodel->setHorizontalHeaderItem(40, new QStandardItem(wtr(L"页面完整URL匹配2")));
	mmodel->setHorizontalHeaderItem(41, new QStandardItem(wtr(L"页面完整正则2")));
	mmodel->setHorizontalHeaderItem(42, new QStandardItem(wtr(L"多hostURL匹配")));
	mmodel->setHorizontalHeaderItem(43, new QStandardItem(wtr(L"多host替换表达式")));
	mmodel->setHorizontalHeaderItem(44, new QStandardItem(wtr(L"多host URL头不满足匹配")));
    mmodel->setHorizontalHeaderItem(45, new QStandardItem(wtr(L"大文件存储(1分段获取;2连续写文件;)")));
    mmodel->setHorizontalHeaderItem(46, new QStandardItem(wtr(L"状态值用URL正则表达式(URL匹配正则式`任务状态值;...)")));

    mmodel->setHorizontalHeaderItem(47, new QStandardItem(wtr(L"抓URL正则规则(URL提取正则表达式`URL替换式`时间替换式`标题替换式`任务状态值;...)")));
    mmodel->setHorizontalHeaderItem(48, new QStandardItem(wtr(L"自动抓取连接方式(1禁止;2链接到本地;3无获取则链接到本地;4链接到网络;5无获取则链接到网络)")));
    mmodel->setHorizontalHeaderItem(49, new QStandardItem(wtr(L"URL匹配则替换(正则表达式`替换式;...)")));
    mmodel->setHorizontalHeaderItem(50, new QStandardItem(wtr(L"内存下载文件大小")));

	setModel(mmodel);
	setColumnWidth(0, 200);
	setColumnWidth(1, 200);
	setColumnWidth(2, 200);
	setColumnWidth(3, 200);
	setColumnWidth(4, 80);
	setColumnWidth(5, 80);
	setColumnWidth(6, 80);
	setColumnWidth(7, 80);
	setColumnWidth(8, 80);
	setColumnWidth(9, 80);
	setColumnWidth(10, 80);
	setColumnWidth(11, 80);
	setColumnWidth(12, 80);
	setColumnWidth(13, 80);
	setColumnWidth(14, 80);
	setColumnWidth(15, 80);
	setColumnWidth(16, 80);
	setColumnWidth(17, 80);
	setColumnWidth(18, 80);
	setColumnWidth(19, 80);
	setColumnWidth(20, 80);
	setColumnWidth(21, 80);
	setColumnWidth(22, 80);
	setColumnWidth(23, 80);
	setColumnWidth(24, 80);
	setColumnWidth(25, 80);
	setColumnWidth(26, 80);
	setColumnWidth(27, 80);
	setColumnWidth(28, 80);
	setColumnWidth(29, 80);
	setColumnWidth(30, 80);
	setColumnWidth(31, 80);

	setColumnWidth(32, 80);
	setColumnWidth(33, 80);
	setColumnWidth(34, 80);
	setColumnWidth(35, 80);
	setColumnWidth(36, 80);
	setColumnWidth(37, 80);

	setColumnWidth(38, 80);
	setColumnWidth(39, 80);
	setColumnWidth(40, 80);
	setColumnWidth(41, 80);

	setColumnWidth(42, 80);
	setColumnWidth(43, 80);
	setColumnWidth(44, 80);
	setColumnWidth(45, 80);
	setColumnWidth(46, 80);
	setColumnWidth(47, 80);
	setColumnWidth(48, 80);
	setColumnWidth(49, 80);
	setColumnWidth(50, 80);

    setEditTriggers(QAbstractItemView::EditKeyPressed);
	//setEditTriggers(QAbstractItemView::AllEditTriggers);
	auto delegate = new AutoEditTreeViewDelegate(this);
	connect(delegate, SIGNAL(EditorClosed()), this, SLOT(EditorClosed()));
	setItemDelegate(delegate);

	connect(delegate, SIGNAL(editMoveLeft()), this, SLOT(editMoveLeft()));
	connect(delegate, SIGNAL(editMoveRight()), this, SLOT(editMoveRight()));
	connect(delegate, SIGNAL(editMoveUp()), this, SLOT(editMoveUp()));
	connect(delegate, SIGNAL(editMoveDown()), this, SLOT(editMoveDown()));

	mtm = new QTimer();
	connect(mtm, SIGNAL(timeout()), this, SLOT(mtmtimeout()));
}

AutoEditTreeView::~AutoEditTreeView()
{
    
}

void AutoEditTreeView::focusInEvent(QFocusEvent * event)
{
    QTreeView::focusInEvent(event);
}


void AutoEditTreeView::focusOutEvent(QFocusEvent * event)
{
    QTreeView::focusInEvent(event);
}

void AutoEditTreeView::keyPressEvent(QKeyEvent *keyevent)
{
		int uKey = keyevent->key();
		Qt::Key key = static_cast<Qt::Key>(uKey);

		Qt::KeyboardModifiers modifiers = keyevent->modifiers();
		//判断某个修饰键是否被按下;
		if (key == Qt::Key_Left)
		{
			auto ind = currentIndex();
			if (ind.isValid())
			{
				if (ind.column() - 1 >= 0)
				{
					ind = ind.sibling(ind.row(), ind.column() - 1);
					setCurrentIndex(ind);
				}
			}
			//update();
			return;
		}
		if (key == Qt::Key_Right)
		{
			auto ind = currentIndex();
			if (ind.isValid())
			{
				if (ind.column() + 1 < (model())->columnCount())
				{
					ind = ind.sibling(ind.row(), ind.column() + 1);
					setCurrentIndex(ind);
				}
			}
			//update();
			return;
		}
		if (key == Qt::Key_Up)
		{
			auto ind = currentIndex();
			if (ind.isValid())
			{
				if (ind.row() - 1 >= 0)
				{
					ind = ind.sibling(ind.row()-1, ind.column());
					setCurrentIndex(ind);
				}
			}
			update();
			return;
		}
		if (key == Qt::Key_Down)
		{
			auto ind = currentIndex();
			if (ind.isValid())
			{
				if (ind.row() + 1 < (model())->rowCount())
				{
					ind = ind.sibling(ind.row()+1, ind.column());
					setCurrentIndex(ind);
				}
			}
			update();
			return;
		}
		if (key == Qt::Key_Delete)
		{
			auto ind = currentIndex();
			if (ind.isValid())
			{
				((QStandardItemModel*)model())->removeRow(ind.row());
			}

			update();
			return;
		}
		/*

		//获取修饰键(Ctrl,Alt,Shift)的状态
		Qt::KeyboardModifiers modifiers = keyevent->modifiers();
		//判断某个修饰键是否被按下
		if(modifiers & Qt::ShiftModifier)
		uKey += Qt::SHIFT;
		if(modifiers & Qt::ControlModifier)
		uKey += Qt::CTRL;
		if(modifiers & Qt::AltModifier)
		uKey += Qt::ALT;
		*/



		//popup information
		QTreeView::keyPressEvent(keyevent);
	
}


void AutoEditTreeView::editorDestroyed(QObject * editor)
{
	QTreeView::editorDestroyed(editor);
}
//
//bool AutoEditTreeView::edit(const QModelIndex & index, EditTrigger trigger, QEvent * event)
//{
//	return false;
//}

void AutoEditTreeView::closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint)
{
	QTreeView::closeEditor(editor,hint);
	
}

void AutoEditTreeView::setModel(QAbstractItemModel * pmodel)
{
	mmodel = (QStandardItemModel*)pmodel;
	QList<QStandardItem*> row;
	for (int i = 0; i < pmodel->columnCount(); i++)
	{
		row.append(new QStandardItem(""));
	}
	((QStandardItemModel*)pmodel)->appendRow(row);
	QTreeView::setModel(pmodel);
}
void AutoEditTreeView::ClearAndNew()
{
	((QStandardItemModel*)model())->removeRows(0, ((QStandardItemModel*)model())->rowCount()-1);
}

void AutoEditTreeView::EditorClosed()
{

	int rowcnt = ((QStandardItemModel*)model())->rowCount();
	if (rowcnt > 0)
	{
		bool blastlineempty = true;
		auto lastitem = ((QStandardItemModel*)model())->item(rowcnt-1,0);
		if (lastitem)
		{
			for (int coli = 0; coli < ((QStandardItemModel*)model())->columnCount(); coli++)
			{
				if (((QStandardItemModel*)model())->item(rowcnt - 1, coli) != 0 && !((QStandardItemModel*)model())->item(rowcnt - 1, coli)->text().isEmpty())
				{
					blastlineempty = false;
					break;
				}
			}
		}
		if (!blastlineempty)
		{
			QList<QStandardItem*> row;
			for (int i = 0; i < ((QStandardItemModel*)model())->columnCount(); i++)
			{
				row.append(new QStandardItem(""));
			}
			((QStandardItemModel*)model())->appendRow(row);
		}
	}
}

void AutoEditTreeView::mtmtimeout()
{
	setCurrentIndex(nexteditind);
	mtm->stop();
	edit(currentIndex());
}

void AutoEditTreeView::DeleteSelectedRow()
{
	auto selinds=this->selectedIndexes();
	
	QStandardItemModel *mmodel = (QStandardItemModel *)model();
	for (int i = 0; i < mmodel->rowCount(); i++)
	{
		auto bdel = false;
		for (int j = 0; j < selinds.size(); j++)
		{
			if (selinds[i].row() == i)
			{
				bdel = true;
				break;
			}
		}
		if (bdel)
		{
			mmodel->removeRow(i);
		}
	}
}

void AutoEditTreeView::AppenEmptyRow()
{
	QList<QStandardItem*> row;
	for (int i = 0; i < ((QStandardItemModel*)model())->columnCount(); i++)
	{
		row.append(new QStandardItem(""));
	}
	((QStandardItemModel*)model())->appendRow(row);
}

