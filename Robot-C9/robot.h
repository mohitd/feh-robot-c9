#ifndef ROBOT_H
#define ROBOT_H

#include "drivetrain.h"
#include "cdscell.h"
#include "cage.h"
#include "diagnostics.h"

class Robot
{
    friend class Diagnostics;
public:
    Robot(DriveTrain& driveTrain, CdSCell& cdsCell, Cage& cage) : driveTrain(driveTrain), cdsCell(cdsCell), cage(cage)
    {

    }
private:
    DriveTrain& driveTrain;
    CdSCell& cdsCell;
    Cage& cage;
};

#endif // ROBOT_H
