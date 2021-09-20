#pragma once

#include <QObject>
#include <QtPlugin>
#include "PlotJuggler/dataloader_base.h"

using namespace PJ;

class DataLoadCustomCsv : public DataLoader {
Q_OBJECT
    Q_PLUGIN_METADATA(IID "facontidavide.PlotJuggler3.DataLoader")
    Q_INTERFACES(PJ::DataLoader)

public:
    DataLoadCustomCsv();

    virtual ~DataLoadCustomCsv() = default;

    virtual const std::vector<const char *> &compatibleFileExtensions() const override;

    virtual bool readDataFromFile(PJ::FileLoadInfo *fileload_info, PlotDataMapRef &destination) override;

    virtual const char *name() const override { return "DataLoad Custom CSV"; }

    virtual bool xmlSaveState(QDomDocument &doc, QDomElement &parent_element) const override;

    virtual bool xmlLoadState(const QDomElement &parent_element) override;

protected:
    QSize parseHeader(QFile *file, std::vector<std::string> &ordered_names);

private:
    std::vector<const char *> _extensions;
    std::string _default_time_axis;
    QChar _separator = QChar(';');
    QChar _comments = QChar('#');
};
