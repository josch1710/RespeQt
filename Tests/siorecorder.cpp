#include "siorecorder.h"
#include "mainwindow.h"
#include "make_unique.h"

#include <QFileDialog>

namespace Tests {
    SioRecorderPtr SioRecorder::sInstance = std::make_shared<SioRecorder>();

    void SioRecorder::startSIOSnapshot(QFile* file)
    {
        if (file->isWritable() && file->isOpen())
        {
            if (!mSnapshotWriter)
            {
                mSnapshotWriter = std::make_unique<QXmlStreamWriter>(file);
            }
            mSnapshotWriter->setAutoFormatting(true);
            mSnapshotWriter->writeStartDocument();
            mSnapshotWriter->writeStartElement("testcase");
            mSnapshotRunning = true;
        }
    }

    void SioRecorder::stopSIOSnapshot()
    {
        if (mSnapshotWriter)
        {
            mSnapshotWriter->writeEndElement();
            mSnapshotWriter->writeEndDocument();
            mSnapshotWriter->device()->close();
            mSnapshotWriter.reset();
            mSnapshotRunning = false;
        }
    }

    void SioRecorder::writeSnapshotCommandFrame(const qint8 no, const qint8 command, const qint8 aux1, const qint8 aux2)
    {
        // Record the command frame, if the snapshot is running
        if (mSnapshotRunning && mSnapshotWriter)
        {
            mSnapshotWriter->writeStartElement("commandframe");
            mSnapshotWriter->writeAttribute("device", QString::number(no));
            mSnapshotWriter->writeAttribute("command", QString::number(command));
            mSnapshotWriter->writeAttribute("aux1", QString::number(aux1));
            mSnapshotWriter->writeAttribute("aux2", QString::number(aux2));
            mSnapshotWriter->writeEndElement();
        }
    }

    void SioRecorder::writeSnapshotDataFrame(const QByteArray &data)
    {
        // Record the command frame, if the snapshot is running
        if (mSnapshotRunning && mSnapshotWriter)
        {
            mSnapshotWriter->writeStartElement("dataframe");
            QString cs;
            //foreach(char c, data)
            for(const unsigned char chr: data)
            {
                cs.append(chr);
                //cs.append("&#");
                //cs.append(QString::number(chr, 10));
                //cs.append(";");
            }
            mSnapshotWriter->writeCharacters(cs);
            mSnapshotWriter->writeEndElement();
        }
    }

}
