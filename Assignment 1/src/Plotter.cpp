#include "Plotter.h"
#include "strlib.h"
using namespace std;

struct Location {
    double x;
    double y;
};

void moveAbs(Location& p, string& line, const PenStyle& pstyle, const bool& state);
void moveRel(Location& p, string& line, const PenStyle& pstyle, const bool& state);

void runPlotterScript(istream& input) {
    /* TODO: Delete this line and the one after it, then implement this function.
     * (void) input;
     */

    PenStyle pstyle = { 1, "black" };
    bool state = false;
    Location p = { 0, 0 };

    for (string line; getline(input, line); ) {

        if (startsWith(toLowerCase(line), "pendown")) {
            state = true;
        } else if (startsWith(toLowerCase(line), "penup")) {
            state = false;
        } else if (startsWith(toLowerCase(line), "moveabs")) {
            moveAbs(p, line, pstyle, state);
        } else if (startsWith(toLowerCase(line), "moverel")) {
            moveRel(p, line, pstyle, state);
        } else if (startsWith(toLowerCase(line), "pencolor")) {
            Vector<string> strList = stringSplit(line, " ");
            pstyle.color = strList[1];

        } else if (startsWith(toLowerCase(line), "penwidth")) {
            Vector<string> strList = stringSplit(line, " ");
            pstyle.width = stringToReal(strList[1]);
        }

    }

}

void moveAbs(Location& p, string& line, const PenStyle& pstyle, const bool& state) {
    Vector<string> strList = stringSplit(line, " ");
    if (state == true) {
        drawLine(p.x, p.y, stringToReal(strList[1]), stringToReal(strList[2]), pstyle);
    }
    p.x = stringToReal(strList[1]);
    p.y = stringToReal(strList[2]);

}


void moveRel(Location& p, string& line, const PenStyle& pstyle, const bool& state) {
    Vector<string> strList = stringSplit(line, " ");
    if (state == true) {
        drawLine(p.x, p.y, p.x+stringToReal(strList[1]), p.y+stringToReal(strList[2]), pstyle);
    }
    p.x += stringToReal(strList[1]);
    p.y += stringToReal(strList[2]);
}
