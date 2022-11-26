#ifndef AutoEditTreeView_H
#define AutoEditTreeView_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QTimer>


class AutoEditTreeViewDelegate :public QItemDelegate
{
	Q_OBJECT
public:
	AutoEditTreeViewDelegate(QObject* parent);
	QWidget* AutoEditTreeViewDelegate::createEditor(QWidget* parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData(QWidget* editor,
		QAbstractItemModel* model,
		const QModelIndex& index
		) const;
	void updateEditorGeometry(QWidget* editor,
		const QStyleOptionViewItem& option,
		const QModelIndex& index
		) const;
	public slots:
	void MycloseEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint = NoHint);
	
protected:
	bool editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index);

signals:
	void EditorClosed();
	void editMoveLeft();
	void editMoveRight();
	void editMoveUp();
	void editMoveDown();
private:
	QString m_lastedittext;
};



class ANameEdit;
class AutoEditTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit AutoEditTreeView(QWidget *parent = 0);
    virtual ~AutoEditTreeView();


	void setModel(QAbstractItemModel * model);
	void ClearAndNew();
	void AppenEmptyRow();
	QStandardItemModel *model(){ return mmodel; }
protected:
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void keyPressEvent(QKeyEvent *keyevent);
	
	void editorDestroyed(QObject * editor);
	//bool edit(const QModelIndex & index, EditTrigger trigger, QEvent * event);
	void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint);

private slots:
	void EditorClosed();

	void editMoveLeft()
	{
		auto ind = currentIndex();
		if (ind.column() - 1 >= 0)
		{
			nexteditind = ind.sibling(ind.row(), ind.column() - 1);
			mtm->start(200);
		}
	}
	void editMoveRight(){
		auto ind = currentIndex();
		if (ind.column() + 1 < model()->columnCount())
		{
			nexteditind = ind.sibling(ind.row(), ind.column() + 1);
			mtm->start(200);
		}
	}
	void editMoveUp(){
		auto ind = currentIndex();
		if (ind.row() - 1 >=0)
		{
			nexteditind = ind.sibling(ind.row() - 1, ind.column());
			mtm->start(200);
		}
	}
	void editMoveDown(){
		auto ind = currentIndex();
		if (ind.row() + 1 < model()->rowCount())
		{
			nexteditind = ind.sibling(ind.row() + 1, ind.column());
			mtm->start(200);
		}
	}
	void DeleteSelectedRow();
public slots:
	void mtmtimeout();

signals:
    

private:

	QStandardItemModel *mmodel;
    QTimer *mtm;
	QModelIndex nexteditind;
};

#endif

