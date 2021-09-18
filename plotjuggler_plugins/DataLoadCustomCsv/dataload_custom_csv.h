#pragma once

#include <QObject>
#include <QtPlugin>
#include "PlotJuggler/dataloader_base.h"

using namespace PJ;

//static QMap<QString, QRegularExpression> gmtFormats {{"%j-%H:%M:%S-%f.%f", QRegularExpression("^(?<j>[0-3][0-9][0-9])-(?<H>[0-2][0-9]):(?<M>[0-5][0-9]):(?<S>[0-5][0-9])-(?<f>[0-9]{3}.[0-9]{3})$", QRegularExpression::NoPatternOption)},
//                                                     {"%j-%H:%M:%S-%f", QRegularExpression("^(?<j>[0-3][0-9]{2})-(?<H>[0-2][0-9]):(?<M>[0-5][0-9]):(?<S>[0-5][0-9])-(?<f>[0-9]{3})$", QRegularExpression::NoPatternOption)},
//                                                     {"%j-%H:%M:%S:%f", QRegularExpression("^(?<j>[0-3][0-9]{2})-(?<H>[0-2][0-9]):(?<M>[0-5][0-9]):(?<S>[0-5][0-9]):(?<f>[0-9]{3})$", QRegularExpression::NoPatternOption)},
//                                                     {"%j-%H:%M:%S", QRegularExpression("^(?<j>[0-3][0-9]{2})-(?<H>[0-2][0-9]):(?<M>[0-5][0-9]):(?<S>[0-5][0-9])$", QRegularExpression::NoPatternOption)},
//                                                     {"hh:mm:ss:zzz", QRegularExpression("^(?<H>[0-2][0-9]):(?<M>[0-5][0-9]):(?<S>[0-5][0-9]):(?<f>[0-9]{3})$", QRegularExpression::NoPatternOption)}};

class DataLoadCustomCsv : public DataLoader
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "facontidavide.PlotJuggler3.DataLoader")
  Q_INTERFACES(PJ::DataLoader)

public:
  DataLoadCustomCsv();
  virtual ~DataLoadCustomCsv() = default;
  virtual const std::vector<const char*>& compatibleFileExtensions() const override;
  virtual bool readDataFromFile(PJ::FileLoadInfo* fileload_info, PlotDataMapRef& destination) override;
  virtual const char* name() const override { return "DataLoad Custom CSV"; }
  virtual bool xmlSaveState(QDomDocument& doc, QDomElement& parent_element) const override;
  virtual bool xmlLoadState(const QDomElement& parent_element) override;

protected:
  QSize parseHeader(QFile* file, std::vector<std::string>& ordered_names);

private:
  std::vector<const char*> _extensions;
  std::string _default_time_axis;
};
