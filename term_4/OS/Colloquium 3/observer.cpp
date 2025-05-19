#include <QObject>
#include <QDebug>

class Sensor : public QObject
{
    Q_OBJECT

signals:
    void temperatureChanged(int value);

public:
    void changeTemperature(int value)
    {
        emit temperatureChanged(value);
    }
};

class Display : public QObject
{
    Q_OBJECT

public slots:
    void updateDisplay(int value)
    {
        qDebug() << "Temperature: " << value;
    }
};


#include <QCoreApplication>
#include "observer_qt.cpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Sensor sensor;
    Display display;

    QObject::connect(&sensor, &Sensor::temperatureChanged,
                     &display, &Display::updateDisplay);

    sensor.changeTemperature(25);
    sensor.changeTemperature(30);

    return 0;
}
