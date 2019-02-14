#include "mapwidget.h"
#include "ui_mapwidget.h"
//*********************************************************************************************************************
MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapWidget),
    mapLoaded(false)
{
    ui->setupUi(this);
}
//*********************************************************************************************************************
MapWidget::~MapWidget()
{
    delete ui;
}
//*********************************************************************************************************************
void MapWidget::update(Coordinates playerCrds)
{
    this->playerCrds = playerCrds;
    repaint();
}
//*********************************************************************************************************************
void MapWidget::loadMap(const std::vector<std::vector<SimpleSpaceObject>>& map)
{
    fields = map;
    mapLoaded = true;
    auto w = static_cast<int>(fieldSize * fields[0].size());
    auto h = static_cast<int>(fieldSize * fields.size());
    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(w, h);
}
//*********************************************************************************************************************
void MapWidget::unloadMap()
{
    mapLoaded = false;
    setMinimumSize(0, 0);
    repaint();
}
//*********************************************************************************************************************
void MapWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    if (mapLoaded)
    {
        auto beginCrds = fields[0][0].crds;
        for (const auto& row: fields)
        {
            for (const auto& field: row)
            {
                if (field.height > 0)
                {
                    auto x = static_cast<unsigned int>(field.crds.x - beginCrds.x);
                    auto y = static_cast<unsigned int>(field.crds.z - beginCrds.z);
                    int beginX = static_cast<int>(fieldSize * x);
                    int beginY = static_cast<int>(fieldSize * y);
                    int endX = static_cast<int>(fieldSize * (x + 1) - 2);
                    int endY = static_cast<int>(fieldSize * (y + 1) - 2);

                    painter.drawLine(beginX, beginY, endX, beginY);
                    painter.drawLine(endX, beginY, endX, endY);
                    painter.drawLine(endX, endY, beginX, endY);
                    painter.drawLine(beginX, endY, beginX, beginY);
                }
            }
        }
        int playerBeginX = static_cast<int>(fieldSize * (playerCrds.x + 0.5f) - playerFieldRadius);
        int playerBeginY = static_cast<int>(fieldSize * (playerCrds.z + 0.5f) - playerFieldRadius);
        int size = static_cast<int>(playerFieldRadius * 2 + 1);
        painter.fillRect(playerBeginX, playerBeginY, size, size, QColor(255, 255, 0));
        painter.drawRect(playerBeginX, playerBeginY, size, size);
    }
}
//*********************************************************************************************************************
