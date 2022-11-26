#ifndef RocksDbDownTaskThread_H
#define RocksDbDownTaskThread_H

#include <QThread>
#include <cstdint>



class RocksDbDownTaskThread:public QThread
{
	Q_OBJECT
public:
	RocksDbDownTaskThread(QString savepath, QString inc_ls, QString exc_ls);
	~RocksDbDownTaskThread();

	
public slots:
	virtual void	run();
	void Stop(){ mStop = true; }
signals:
	void executeResult(uint32_t graphfuncid, QByteArray result);
	void NeedDelThread(QThread *thrd);
private:
	QByteArray executedata;
	QString funcpath, savepath, inc_ls, exc_ls;
    bool mStop;
	uint32_t graphfuncid;
};


#endif


