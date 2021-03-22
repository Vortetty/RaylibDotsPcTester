/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <random>
#include <vector>
#include <set>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <glfw3.h>
#include <deque>

#define byte char
namespace fs = std::filesystem;


bool inRange(float low, float high, float x)
{
    return (low < x && x < high);
}

int32_t randint32_t(int32_t a, int32_t b){
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return (int32_t)(a + r);
}

float randFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

class dot{
    public:
        Vector2 vel, pos, start;

        dot(int32_t screenWidth, int32_t screenHeight){
            switch(randint32_t(0, 4)){
                case 0: // Starts on bottom
                    pos.x = randFloat(0+50, screenWidth-50);
                    pos.y = 0;
                    vel.x = randFloat(-1.5, 1.5); //Random -1 to 1 velocity on x
                    vel.y = randFloat(1, 1.5); //Random -1 to 1 velocity on y
                    if (vel.x == 0) vel.x = 1;
                    if (vel.y == 0) vel.y = 1;
                    break;
                case 1: // Starts on top
                    pos.x = randFloat(0+50, screenWidth-50);
                    pos.y = screenHeight-1;
                    vel.x = randFloat(-1.5, 1.5); //Random -1 to 1 velocity on x
                    vel.y = -randFloat(1, 1.5); //Random -1 to 1 velocity on y
                    if (vel.x == 0) vel.x = -1;
                    if (vel.y == 0) vel.y = -1;
                    break;
                case 2: // Starts on left
                    pos.x = 0;
                    pos.y = randFloat(0+50, screenHeight-50);
                    vel.x = randFloat(1, 1.5); //Random -1 to 1 velocity on x
                    vel.y = randFloat(-1.5, 1.5); //Random -1 to 1 velocity on y
                    if (vel.x == 0) vel.x = 1;
                    if (vel.y == 0) vel.y = 1;
                    break;
                case 3: // Starts on right
                    pos.x = screenWidth-1;
                    pos.y = randFloat(0+50, screenHeight-50);
                    vel.x = -randFloat(1, 1.5); //Random -1 to 1 velocity on x
                    vel.y = randFloat(-1.5, 1.5); //Random -1 to 1 velocity on y
                    if (vel.x == 0) vel.x = -1;
                    if (vel.y == 0) vel.y = -1;
                    break;
            };

            start.x = pos.x;
            start.y = pos.y;
        };

        dot(float vx, float vy, int32_t sx, int32_t sy){
            vel.x = vx;
            vel.y = vy;
            pos.x = sx;
            pos.y = sy;
            start.x = pos.x;
            start.y = pos.y;
        };

        void move(int32_t screenWidth, int32_t screenHeight){
            if (vel.x == 0) vel.x = randFloat(-1.5, 1.5);
            if (vel.y == 0) vel.y = randFloat(-1.5, 1.5);
            
            pos.x = pos.x + vel.x;
            pos.y = pos.y + vel.y;

            if (!inRange(0, screenWidth, pos.x) || !inRange(0, screenHeight, pos.y)) {
                pos.x = start.x;
                pos.y = start.y;
            }
        }
};

float distance(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2) + 
                pow(y2 - y1, 2) * 1.0);
}

struct line {
    Vector2 v1, v2;
};

bool vec_equal(Vector2 &a, Vector2 &b) {
    return a.x==b.x&&a.y==b.y;
}
bool line_equal(line &a, line &b) {
    return vec_equal(a.v1, b.v1)&&vec_equal(a.v2, b.v2);
}

int32_t wrap(int32_t X, int32_t const lowerBound, int32_t const upperBound)
{
    int32_t range_size = upperBound - lowerBound + 1;

    if (X < lowerBound)
        X += range_size * ((lowerBound - X) / range_size + 1);

    return lowerBound + (X - lowerBound) % range_size;
}

std::vector<fs::path> getByExt(fs::path const & root, std::string const & ext)
{
    std::vector<fs::path> paths;

    if (fs::exists(root) && fs::is_directory(root))
    {
        for (auto const & entry : fs::recursive_directory_iterator(root))
        {
            if (fs::is_regular_file(entry) && entry.path().extension() == ext)
                paths.emplace_back(entry.path().filename());
        }
    }

    return paths;
} 

static std::deque<char> readAllBytes(char const* filename)
{
    std::ifstream ifs(filename, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::deque<char> result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&result[0], pos);

    return result;
}

int32_t main(int32_t argc, char* argv[])
{
    srand(0);
    // Initialization
    //--------------------------------------------------------------------------------------
    int32_t screenWidth = 800;
    int32_t screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib dot field pc test");
    
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Please wait, Initializing.", 10, 10, 20, LIME);
    EndDrawing();

    int32_t currentBalls = 1;
    double timeToDraw = 0;
    double tempTime = GetTime();
    int32_t maxfps = 0;
    std::vector<dot> balls;
    bool firstFrameRun = false;
    bool configDone = false;
    bool closeNow = false;
    bool closeFileViewer = false;

    int32_t frameSettings[] = { 1, 5, 10, 15, 30, 60, 120 };
    byte frame = 5;
    int32_t addBallNFramesSettings[] = { 1, 5, 10, 15, 30, 60 };
    byte addBallNFrames = 3;
    int32_t ballsAddedPerFrameSettings[] = { 1, 2, 3, 4, 5 };
    byte ballsAddedPerFrame = 0;
    int32_t lineWidthSettings[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    byte lineWidth = 2;
    int32_t selectedSetting = 0;


    SetTargetFPS(30);
    while (!closeNow && !configDone){
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawText(TextFormat("Stop at framerate < %i >", frameSettings[frame]), 10, 10, 20, selectedSetting == 0 ? LIME : BLACK);
        DrawText(TextFormat("Add ball(s) every < %i > frame(s)", addBallNFramesSettings[addBallNFrames]), 10, 40, 20, selectedSetting == 1 ? LIME : BLACK);
        DrawText(TextFormat("Add < %i > ball(s) every %i frame(s)", ballsAddedPerFrameSettings[ballsAddedPerFrame], addBallNFramesSettings[addBallNFrames]), 10, 70, 20, selectedSetting == 2 ? LIME : BLACK);
        DrawText(TextFormat("Draw lines with width < %i >", lineWidthSettings[lineWidth]), 10, 100, 20, selectedSetting == 3 ? LIME : BLACK);

        DrawText("Configuration, press enter to begin test\nUp/Down to select setting, Left/Right to change setting\nDrop an rdts file here to view the results it stored.", 10, screenHeight-85, 20, LIME);

        switch (GetKeyPressed()){
            case KEY_ENTER:
                configDone = true;
                break;
            case KEY_UP:
                selectedSetting = wrap(selectedSetting-1, 0, 3);
                break;
            case KEY_DOWN:
                selectedSetting = wrap(selectedSetting+1, 0, 3);
                break;
            case KEY_RIGHT:
                switch (selectedSetting){
                    case 0:
                        frame = wrap(frame+1, 0, sizeof(frameSettings)/sizeof(frameSettings[0])-1);
                        break;
                    case 1:
                        addBallNFrames = wrap(addBallNFrames+1, 0, sizeof(addBallNFramesSettings)/sizeof(addBallNFramesSettings[0])-1);
                        break;
                    case 2:
                        ballsAddedPerFrame = wrap(ballsAddedPerFrame+1, 0, sizeof(ballsAddedPerFrameSettings)/sizeof(ballsAddedPerFrameSettings[0])-1);
                        break;
                    case 3:
                        lineWidth = wrap(lineWidth+1, 0, sizeof(lineWidthSettings)/sizeof(lineWidthSettings[0])-1);
                        break;
                }
                break;
            case KEY_LEFT:
                switch (selectedSetting){
                    case 0:
                        frame = wrap(frame-1, 0, sizeof(frameSettings)/sizeof(frameSettings[0])-1);
                        break;
                    case 1:
                        addBallNFrames = wrap(addBallNFrames-1, 0, sizeof(addBallNFramesSettings)/sizeof(addBallNFramesSettings[0])-1);
                        break;
                    case 2:
                        ballsAddedPerFrame = wrap(ballsAddedPerFrame-1, 0, sizeof(ballsAddedPerFrameSettings)/sizeof(ballsAddedPerFrameSettings[0])-1);
                        break;
                    case 3:
                        lineWidth = wrap(lineWidth-1, 0, sizeof(lineWidthSettings)/sizeof(lineWidthSettings[0])-1);
                        break;
                }
                break;
        }

        EndDrawing();

        if (WindowShouldClose()) closeNow = true;
        if (IsFileDropped()) {
            int files = 0;
            char* file = GetDroppedFiles(&files)[0];
            std::deque save = readAllBytes(file);

            // Save file will be aligned to 4 bytes in the order as follows
            // 00 00 00 00 | Highest Balls
            // 00 00 00 00 | Highest FPS
            // 00 00 00 00 | Hours taken to run
            // 00 00 00 00 | Minutes taken to run
            // 00 00 00 00 | Seconds taken to run
            // 00 00 00 00 | Settings, each taking 1 byte and representing index in the settings list, follows the order: 
            //                    [Stop at framerate, Frames to add ball(s), Balls added per n frames, Line width]
            // 00*N        | String showing gpu model, undefined length

            int maxBalls = (unsigned char)save[3] << 24 | (unsigned char)save[2] << 16 | (unsigned char)save[1] << 8 | (unsigned char)save[0];
            int maxFPS = (unsigned char)save[3+4] << 24 | (unsigned char)save[2+4] << 16 | (unsigned char)save[1+4] << 8 | (unsigned char)save[0+4];
            int hours = (unsigned char)save[3+8] << 24 | (unsigned char)save[2+8] << 16 | (unsigned char)save[1+8] << 8 | (unsigned char)save[0+8];
            int mins = (unsigned char)save[3+12] << 24 | (unsigned char)save[2+12] << 16 | (unsigned char)save[1+12] << 8 | (unsigned char)save[0+12];
            int secs = (unsigned char)save[3+16] << 24 | (unsigned char)save[2+16] << 16 | (unsigned char)save[1+16] << 8 | (unsigned char)save[0+16];
            byte stopFrames = save[3+20];
            byte ballsEveryNFrames = save[2+20];
            byte ballsAddedEveryNFrames = save[1+20];
            byte lineThickness = save[0+20];

            for (int i = 0; i < 24; i++) save.pop_front();

            std::string gpuModel(save.begin(), save.end());

            while (!closeNow && !closeFileViewer) {
                switch (GetKeyPressed()){
                    case KEY_ENTER:
                        closeFileViewer = true;
                        break;
                }

                
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText(TextFormat("GPU: %s\n\nStats:\n Balls reached: %i\n Max FPS: %i\n Time to run: %i:%i:%i\n\nSettings:\n Stop at framerate %i\n Add ball(s) every %i frame(s)\n Add %i ball(s) every %i frame(s)\n Draw lines with width %i", gpuModel.c_str(), maxBalls, maxFPS, hours, mins, secs, frameSettings[stopFrames], addBallNFramesSettings[ballsEveryNFrames], ballsAddedPerFrameSettings[ballsAddedEveryNFrames], lineWidthSettings[lineThickness]), 10, 10, 20, selectedSetting == 3 ? LIME : BLACK);

                DrawText("Press enter to return to configuration", 10, screenHeight-25, 20, LIME);
                EndDrawing();
                if (WindowShouldClose()) closeNow = true;
            }
        }
    }
    SetTargetFPS(0);

    for (int32_t i = 0; i < ballsAddedPerFrameSettings[ballsAddedPerFrame]; i++) balls.push_back(dot(screenWidth, screenHeight));
    currentBalls = ballsAddedPerFrameSettings[ballsAddedPerFrame];

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!closeNow && (GetFPS() >= frameSettings[frame] || !firstFrameRun))    // Detect window close button or ESC key and exit if pressed
    {
        currentBalls += ballsAddedPerFrameSettings[ballsAddedPerFrame];
        for (int32_t i = 0; i < ballsAddedPerFrameSettings[ballsAddedPerFrame]; i++) balls.push_back(dot(screenWidth, screenHeight));

        for (int32_t frames = 0; frames < addBallNFramesSettings[addBallNFrames] && (GetFPS() >= frameSettings[frame] || !firstFrameRun); frames++){
            if (GetFPS() > maxfps) maxfps = GetFPS();

            // Update
            for (auto& i : balls) { 
                i.move(screenWidth, screenHeight);
            }
            
            std::vector<line> lines;

            for (dot b1 : balls) { 
                for (dot b2 : balls) { 
                    if (distance(b1.pos.x, b1.pos.y, b2.pos.x, b2.pos.y) < 50) {
                        lines.push_back(
                            {
                                b1.pos, b2.pos
                            }
                        );
                    }
                }
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();
            ClearBackground(RAYWHITE);

            for (auto& i : balls) { 
                DrawCircle(i.pos.x, i.pos.y, 2, BLACK);
            } 

            for (auto& i : lines) { 
                DrawLineEx(i.v1, i.v2, lineWidthSettings[lineWidth], {0, 0, 0, (unsigned char)(127-distance(i.v1.x, i.v1.y, i.v2.x, i.v2.y)*2.54)});
            } 

            DrawText(TextFormat("%i FPS\n%i Balls", GetFPS(), currentBalls), 10, 10, 20, LIME);
            //DrawText("Test running...\nPress ESC to end test.", 10, screenHeight-55, 20, LIME);
            EndDrawing();
            //----------------------------------------------------------------------------------
        }
        firstFrameRun = true;
        if (WindowShouldClose()) closeNow = true;
    }

    if (!closeNow){
        SetTargetFPS(1);
        int32_t hours = GetTime()/3600;
        int32_t minutes = (int32_t)(GetTime()/60)%60;
        int32_t seconds = (int32_t)GetTime()%60;
        std::string finalTest = TextFormat("Results:\n Balls: %i\n Max FPS: %i\n Time: %i:%i:%i", currentBalls, maxfps, hours, minutes, seconds);

        try {
            auto exts = getByExt(".", ".rdts");

            for (int32_t i = exts.size(); i >= 0; i--){
                fs::rename(TextFormat("%i.rdts", i), TextFormat("%i.rdts", i+1));
            }
        }
        catch (...) {}


        std::ofstream save("0.rdts", std::ios::binary);

        // Save file will be aligned to 4 bytes in the order as follows
        // 00 00 00 00 | Highest Balls
        // 00 00 00 00 | Highest FPS
        // 00 00 00 00 | Hours taken to run
        // 00 00 00 00 | Minutes taken to run
        // 00 00 00 00 | Seconds taken to run
        // 00 00 00 00 | Settings, each taking 1 byte and representing index in the settings list, follows the order: 
        //                    [Stop at framerate, Frames to add ball(s), Balls added per n frames, Line width]
        // 00*N        | String showing gpu model, undefined length

        int32_t settingInt = frame << 24 | addBallNFrames << 16 | ballsAddedPerFrame << 8 | lineWidth;
        auto gfxModel = glGetString(GL_RENDERER);

        save.write(reinterpret_cast<char*>(&currentBalls), sizeof(currentBalls));
        save.write(reinterpret_cast<char*>(&maxfps), sizeof(maxfps));
        save.write(reinterpret_cast<char*>(&hours), sizeof(hours));
        save.write(reinterpret_cast<char*>(&minutes), sizeof(minutes));
        save.write(reinterpret_cast<char*>(&seconds), sizeof(seconds));
        save.write(reinterpret_cast<char*>(&settingInt), sizeof(settingInt));
        save << gfxModel;

        for (int32_t frames = 0; frames < 5 && !closeNow; frames++){
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(finalTest.c_str(), 10, 10, 20, LIME);
            EndDrawing();
            if (WindowShouldClose()) closeNow = true;
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}