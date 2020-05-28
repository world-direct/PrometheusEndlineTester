#ifndef WORLDDIRECT_NAMEPLATE_H
#define WORLDDIRECT_NAMEPLATE_H

#include <QObject>

namespace worlddirect {

    class NamePlate : public QObject
    {
        Q_OBJECT
    public:
        explicit NamePlate(QObject *parent = nullptr);
        NamePlate&  operator= ( const  NamePlate& ) = delete;
        NamePlate&  operator= ( const  NamePlate&& ) = delete;

        NamePlate ( const  NamePlate& ) = delete;
        NamePlate ( const  NamePlate&& ) = delete;

        virtual ~NamePlate() = default;

    public slots:
        void printNamePlate();
        void sendNamePlateInfo();

    private:
        QString m_serialNumber;
        QString m_endpointName;
        QString m_iccId;
    };

} // namespace worlddirect

#endif // WORLDDIRECT_NAMEPLATE_H
