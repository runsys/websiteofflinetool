#ifndef WEBSITEOFFLINETOOL_H
#define WEBSITEOFFLINETOOL_H

#include <QtWidgets/QMainWindow>
#include "ui_websiteofflinetool.h"
#include <QTimer>
#include <QThread>
#include <QTranslator>

class WebsiteOfflineTool : public QMainWindow
{
	Q_OBJECT

public:
	WebsiteOfflineTool(QWidget *parent = 0);
	~WebsiteOfflineTool();

	void SaveData();
	void LoadData();

	void closeEvent(QCloseEvent * event);
public slots:
	void startbtnclicked();
	void threadfinished();
	void actionLebb_cc_triggerred();
	void actionMd5_filename_to_url_filenametriggerred();
	void actionHtml_title_to_filenametriggerred();
	void quittmtimeout();
	void regtip();
	void buybtnclicked();
	void regbtnclicked();
	void actionDelete_if_no_exist_urlclicked();
	void actionRelinkAllFileclicked();
	void actionSave_data_convert_to_rocksdbclicked();
	void tasklisttreeDoubleClick(QModelIndex ind);
	void actionReset_no_filedata_url_stateclicked();
	void actionMin_file_dir_to_rocksdbclicked();
	void actionDelete_min_file_directoryclicked();
	void actionRocksdb_convert_to_file_dbclicked();
	void actionSaveclicked();
	void actionConvert_to_content_dispositon_file_nameclicked();
	void actionDownload_a_test_url_to_test_htmlclicked();
	void actionUpdate_urlstate_by_find_contentclicked();
	void actionDelete_when_urlstate_is_6();
	void actionExport_title_by_url_regex();
	void actionName_list_to_utf8_url_list();
	void actionAddtime_from_html_regex();
	void actionExport_title_by_url_regex_for_xuetutu();
	void actionName_list_to_utf8_url_list_for_xuetutu();
	void actionXueTutuUrlNotSameCorrect();
	void actionMd5_filename_to_url_file_name_without_relink();
	void action_ted_con_fix_vtt_disposition_content_value();
	void action_ted_con_fix_filename();
	void actionUrl_filename_to_url_md5_filename();
private slots:
    void on_actionChinese_triggered();

    void on_actionEnglish_triggered();

    void on_actionSave_triggered();

private:
	Ui::WebsiteOfflineToolClass ui;
    int threadcnt;
    QTimer *quittm;
    int quit;
	QLabel *tip;
	QList<QThread*> thrdls;
	QLabel *info;
    QTranslator* translator;
};

#endif // WEBSITEOFFLINETOOL_H
