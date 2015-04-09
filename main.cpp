#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>

#define TITLE 0
#define BASE 12
#define WATER 13
#define FOREST 14
#define PLAINS 15
#define HILLS 16

#ifdef linux
#define OS_Windows 0

#elif _WIN32
#define OS_Windows 1
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

const int World_Map_X = 24;
const int World_Map_Y = 36; // yes, y is where x should be... odd...

/*
 E = Engineer
 D = Doctor
 S = Scientist
 W = Worker

 Infrequent letters are noticeable:
 Z
 X
 V
 Q
 J

 Bad Letters
 e = Bad to use since it is awkward to read on small screens

 c = Colonist
 n = native
 C = colonist base
 N = native base

*/
std::string world_map[World_Map_X][World_Map_Y] = {
    {"pe", "he", "he","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","we", "we", "we","we", "we", "fe","fe", "fe", "he","we", "pe", "fe","pe", "pe", "pe","pe", "pe", "fe","pe", "he", "he","we", "fe", "fe"},
    {"pe", "pe", "he","he", "fe", "fw","pe", "he", "he","we", "pe", "we","we", "we", "we","we", "we", "fe","fe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "fe","pe", "he", "he","fe", "fe", "fe"},
    {"we", "pe", "fe","hw", "we", "we","pe", "he", "he","we", "we", "we","we", "we", "fe","pe", "we", "we","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "fe","pe", "he", "fe","fe", "fe", "fe"},
    {"we", "pe", "fe","fe", "fe", "we","pe", "pe", "he","pe", "pe", "we","we", "fe", "fe","pe", "we", "we","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "fe","pe", "he", "fe","fe", "fe", "fe"},
    {"we", "we", "fe","fe", "we", "we","pe", "pe", "he","pe", "pe", "fe","we", "fe", "fe","pe", "pe", "we","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "fe","pe", "fe", "fe","fe", "fe", "fe"},
    {"we", "pe", "fe","fe", "fe", "fe","pe", "he", "he","pe", "pe", "we","we", "we", "we","fe", "we", "we","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "fe","pe", "fe", "fe","fe", "fe", "fe"},
    {"pe", "he", "he","pe", "pe", "fe","pe", "pe", "pe","pe", "we", "we","we", "we", "we","we", "we", "we","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "fe","pe", "pe", "fe","fe", "fe", "fe"},
    {"pe", "pe", "he","he", "pe", "fw","pe", "he", "he","pe", "pe", "we","pe", "we", "we","we", "we", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "pe","pe", "fe", "fe","we", "pe", "fe"},
    {"we", "pe", "fe","hw", "we", "we","pe", "fe", "he","pe", "pe", "fe","pe", "he", "we","we", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "pe","pe", "fe", "he","we", "we", "fe"},
    {"we", "pe", "fe","fe", "fe", "we","fe", "fe", "he","fe", "we", "we","pe", "fe", "he","we", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "pe","pe", "he", "he","we", "we", "we"},
    {"we", "we", "fe","fe", "we", "we","fe", "fe", "fe","fe", "we", "we","pe", "pe", "he","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "fe","pe", "pe", "we","we", "we", "we"},
    {"we", "pe", "fe","fe", "fe", "fe","fe", "fe", "fe","we", "we", "we","pe", "pe", "pe","pe", "pe", "bb","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "fe","pe", "pe", "pe","we", "he", "we"},
    {"pe", "he", "he","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "pe", "pe","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "fe", "fe","pe", "pe", "we","he", "he", "fe"},
    {"pe", "pe", "he","he", "fe", "fw","pe", "he", "he","we", "pe", "we","pe", "pe", "he","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","fe", "fe", "fe","pe", "he", "he","he", "he", "we"},
    {"we", "pe", "fe","hw", "we", "we","pe", "he", "he","we", "we", "we","pe", "pe", "pe","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "fe", "fe","pe", "he", "he","he", "pe", "we"},
    {"we", "pe", "fe","fe", "fe", "we","pe", "pe", "he","pe", "pe", "we","pe", "he", "he","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "fe", "fe","pe", "he", "he","he", "pe", "we"},
    {"we", "we", "fe","fe", "we", "we","pe", "pe", "he","pe", "pe", "fe","pe", "he", "he","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "fe", "fe","pe", "he", "he","we", "we", "we"},
    {"we", "pe", "fe","fe", "fe", "fe","pe", "he", "he","pe", "pe", "we","pe", "pe", "pe","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","fe", "fe", "fe","fe", "fe", "fe","pe", "we", "we","we", "pe", "fe"},
    {"pe", "he", "he","pe", "pe", "fe","pe", "pe", "pe","pe", "we", "we","pe", "pe", "pe","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "fe","we", "we", "we","we", "pe", "fe"},
    {"pe", "pe", "he","he", "pe", "fw","pe", "he", "he","pe", "pe", "we", "pe", "fe","hw", "we", "we","pe", "he", "he","we", "we", "we","we","pe", "he", "he","pe", "pe", "fe","pe", "we", "he","we", "we", "fe"},
    {"we", "pe", "fe","hw", "we", "we","pe", "fe", "he","pe", "pe", "fe","pe", "he", "he","pe", "pe", "fe","pe", "he", "he","we", "pe", "fe","pe", "he", "he","pe", "pe", "pe","pe", "we", "we","we", "we", "we"},
    {"we", "pe", "fe","fe", "fe", "we","fe", "fe", "he","fe", "we", "we", "pe", "fe","hw", "we", "we","pe", "he", "he","we", "we", "we","we","pe", "he", "he","pe", "pe", "pe","pe", "he", "we","we", "we", "we"},
    {"we", "we", "fe","fe", "we", "we","fe", "fe", "fe","fe", "we", "we","pe", "he", "he","pe", "pe", "he","he", "fe", "fw","pe", "he", "he","pe", "he", "he","pe", "pe", "pe","pe", "he", "we","we", "we", "fe"},
    {"we", "we", "fe","fe", "we", "we","fe", "fe", "fe","fe", "we", "we","pe", "pe", "he", "he","pe", "pe", "fe","pe", "he", "he","we", "pe","pe", "he", "he","pe", "pe", "pe","pe", "he", "he","we", "pe", "fe"}
    };

int roll(int min, int max){
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(min,max);
    return distribution(generator);
}

bool is_land(std::string world_map[World_Map_X][World_Map_Y]){ if(world_map[World_Map_X][World_Map_Y].front() != 'w'){return true;} else {return false;}}

void generate_world_map(std::string world_map[World_Map_X][World_Map_Y], int x, int y){
    // let there be water
    for(int i = 0; i < x; i++){
        for (int j = 0; j < x; j++){
            world_map[i][j] = "we";
        }
    }
    // let there be continents
    int number_of_continents = roll(1,6);
    for(int i = 0; i < number_of_continents; i++){ // for each continent
        // determine size
        if(number_of_continents == 1){ // super continent case
            int world_map_x = 3; int world_map_y = 3;
            int percent_of_land_of_world = 29;
            int percent_of_desert_of_land = 33;
            int q = world_map_x*world_map_y;
            int w = q / 100;
            int amount_of_land_of_world = w*percent_of_land_of_world;
            int amount_of_desert_of_land = (amount_of_land_of_world/100)*percent_of_desert_of_land;
        }else if(number_of_continents == 2){

        }else if(number_of_continents == 3){

        }else if(number_of_continents == 4){

        }else if(number_of_continents == 5){

        }else if(number_of_continents == 6){

        }
        // determine shape
        int number_defining_shape = roll(1,6);
        if(number_defining_shape == 1){ // shape is circle

        }else if(number_defining_shape == 2){ // shape is square

        }else if(number_defining_shape == 3){ // shape is horizontal rectangle

        }else if(number_defining_shape == 4){ // shape is vertical rectangle

        }else if(number_defining_shape == 5){ // shape is triangle

        }else if(number_defining_shape == 6){ // shape is s

        }
        // determine placement
        if(number_of_continents == 1){ // placement of super continent in the center

        }
        // apply erosion
    }
    // let there be a starting base
    int number_of_land_tiles = 0;
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            if(is_land(world_map)){ number_of_land_tiles++;}
        }
    }
    if(number_of_land_tiles == 0){
        std::cout << "Error: There are no land tiles on this world map." << std::endl;
    }
    // determine starting base location
    int starting_base_rng = roll(1,number_of_land_tiles);
    int iter;
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            if(is_land(world_map)){
                iter++;
                if(iter == starting_base_rng){
                    world_map[i][j] = "bb";
                }
            }
        }
    }
}

std::string getFileContents (std::ifstream& File)
{
    std::string Lines = "";        //All lines

    if (File)                      //Check if everything is good
    {
	while (File.good ())
	{
	    std::string TempLine;                  //Temp line
	    std::getline (File , TempLine);        //Get temp line
	    TempLine += "\n";                      //Add newline character

	    Lines += TempLine;                     //Add newline
	}
	return Lines;
    }
    else                           //Return error
    {
	return "ERROR File does not exist.";
    }
}

void print_skull(){
    using namespace std;
    ifstream Reader ("File1.txt");
    string Art = getFileContents (Reader);
    cout << Art << endl;
    Reader.close ();
}

#ifdef _WIN32
void my_print(std::string s, unsigned colour){
    const WORD colors[] = { 0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F, 0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x04, 0x1F, 0x2F, 0x8F, 0x5F};
    // 0x1A = 0 = title
    // 0x04 = 12 = base
    // 0x1F = 13 = water
    // 0x2F = 14 = forest
    // 0x8F = 15 = plains
    // 0x5F = 16 = hills
    HANDLE hstdin  = GetStdHandle( STD_INPUT_HANDLE  );
	HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
	// Remember how things were when we started
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo( hstdout, &csbi );
	// Draw pretty colors until the user presses any key
    SetConsoleTextAttribute( hstdout, colors[colour]);
    std::cout << s << std::endl;
	FlushConsoleInputBuffer( hstdin );
	// Keep users happy
	SetConsoleTextAttribute( hstdout, csbi.wAttributes );
}

void my_println(std::string s, unsigned colour){
    my_print(s+'\n', colour);
}

void my_printc(char s, unsigned colour){
    const WORD colors[] = { 0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F, 0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x04, 0x1F, 0x2F, 0x8F, 0x5F};
    // 0x1A = 0 = title
    // 0x04 = 12 = base
    // 0x1F = 13 = water
    // 0x2F = 14 = forest
    // 0x8F = 15 = plains
    // 0x5F = 16 = hills
    HANDLE hstdin  = GetStdHandle( STD_INPUT_HANDLE  );
	HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
	// Remember how things were when we started
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo( hstdout, &csbi );
	// Draw pretty colors until the user presses any key
    SetConsoleTextAttribute( hstdout, colors[colour]);
    std::cout << s;
	FlushConsoleInputBuffer( hstdin );
	// Keep users happy
	SetConsoleTextAttribute( hstdout, csbi.wAttributes );
}

#endif
void fHelp(){
    using namespace std;
#ifdef _WIN32
    my_println("\t\t\t\tHELP:\t\t\t\t",0);
#elif linux
    cout << "HELP:" << endl;
#endif
    cout << "General commands | Shortcut: " << endl;
    cout << "-------------------------------------" << endl;
#ifdef _WIN32
    my_println("quit             | q",12);
    my_println("help             | ?",12);
    my_println("look             | l",12);
    my_println("team             | t",12);
    my_println("resources        | r",12);
    my_println("infrastructure   | i",12);
    my_println("science          | sci",12);
    my_println("activity         | a",12);
#elif linux
    cout << "quit             | q" << endl;
    cout << "help             | ?" << endl;
    cout << "look             | l" << endl;
    cout << "team             | t" << endl;
    cout << "resources        | r" << endl;
    cout << "infrastructure   | i" << endl;
    cout << "science          | sci" << endl;
    cout << "activity         | a" << endl;
#endif
    cout << "save             | sv" << endl;
    cout << "end              | x" << endl;
    cout << endl;
    cout << "Direction cmds   | Shortcut: " << endl;
    cout << "-------------------------------------" << endl;
    cout << "north            | n" << endl;
    cout << "east             | e" << endl;
    cout << "south            | s" << endl;
    cout << "west             | w" << endl;
    cout << endl;
}
void display_prologue(){
    using namespace std;
#ifdef _WIN32
    my_println("\t\t\t\t PROLOGUE: \t\t\t\t", 0);
#elif linux
    cout << "PROLOGUE:" << endl;
#endif
    cout << "It is the year 5050 AD. The golden age of human civilisation is now over." << endl;
    cout << "In the century past, the Empire was interstellar." << endl;
    cout << "From their modest roots on Earth, their colonisation spread until they reached the Andromeda Galaxy." << endl;
    cout << "Descending into decadence, inbreeding and apathy, your forebears reached epic levels of mediocrity." << endl;
    cout << "As the descendants of such people, you command a disgruntled and treacherous crew who, for reasons of gross incompetency, crash landed on the most dangerous planet in Andromeda, LV421." << endl;
    cout << "It is time for you to pick up the pieces, or die trying." << endl;
    cout << "Good luck. You will need it." << endl;
    cout << endl;
}

void my_exit(){
    std::string s = "Your time on this world has come to an end...\t\t\t\t";
#ifdef _WIN32
    my_println(s, 0);
#elif linux
    std::cout << s << endl;
#endif // _WIN32
}

void print_tile(std::string s){
    // 0x1A = 0 = title
    // 0x04 = 12 = base
    // 0x1F = 13 = water
    // 0x2F = 14 = forest
    // 0x8F = 15 = plains
    // 0x5F = 16 = hills

    if(s == "bb"){
        my_printc('b', 12);
    }
    if(s.back() == 'e'){ // if empty
        if(s.front() == 'f'){
            my_printc(' ',14);
        }else if (s.front() == 'p'){
            my_printc(' ',15);
        }else if (s.front() == 'w'){
            my_printc(' ',13);
        }else if (s.front() == 'h'){
            my_printc(' ',16);
        }else if (s.front() == 'r'){
            //my_printc(s.back(),);
        }
    }else{ // if not empty, print contents
        if(s.front() == 'f'){
            my_printc(s.back(),14);
        }else if (s.front() == 'p'){
            my_printc(s.back(),15);
        }else if (s.front() == 'w'){
            my_printc(s.back(),13);
        }else if (s.front() == 'h'){
            my_printc(s.back(),16);
        }else if (s.front() == 'r'){
        //my_printc(s.back(),);
        }
    }
}

void print_world_map(std::string world_map[World_Map_X][World_Map_Y], int x, int y){
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            print_tile(world_map[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void my_look(std::string world_map[World_Map_X][World_Map_Y], int x, int y){
    print_world_map(world_map, x, y);
}

void my_team(){
    using namespace std;
    my_println("\t\t\t\t TEAM:\t\t\t\t", 0);
    cout << "Title\t\tFull Name\tSpeciality\tLevel\tXP\tCoord\tMorale" << endl;
    cout << "-----\t\t---------\t----------\t-----\t--\t-----\t------" << endl;
    cout << "Cadet\t\tMatthew Corbins\tPeon\t\t1\t0\t0,0\tApathy" << endl;
    cout << "Lance-Corporal\tPaul Massey\tSecurity\t2\t10\t0,0\tPTSD" << endl;
    cout << "Cadet\t\tThe Jay Man\tMad Scientist\t1\t0\t0,0\tJolly" << endl;
    cout << "Cadet\t\tCarlos\t\tDrama Queen\t1\t0\t0,0\tFeeling ignored" << endl;
    cout << endl;
}

void my_resources(){
    using namespace std;
#ifdef _WIN32
    my_println("\t\t\t\tRESOURCES:\t\t\t\t",0);
#endif // _WIN32
    cout << "Resource\tTotal\tGrowth Rate" << endl;
    cout << "----------\t-----\t----------" << endl;
    cout << "Material\t100\t+10/t" << endl;
    cout << "Biomass\t\t60\t+5/t" << endl;
    cout << "Energy\t\t90\t+9/t" << endl;
    cout << "----------\t----" << endl;
    cout << "Population:\t4/10" << endl;
    cout << "AWOL:\t\t0" << endl;
    cout << endl;
}

void my_inf(){
    using namespace std;
#ifdef _WIN32
    my_println("\t\t\t\tINFRASTRUCTURE:\t\t\t\t",0);
#endif // _WIN32
    cout << "Building\tTier\tPurpose\t\t\t\t\tMaintenance" << endl;
    cout << "Crash site\t0\tInitial home base\t\t\t0/turn" << endl;
}

void my_sci(){
    using namespace std;
#ifdef _WIN32
    my_println("\t\t\t\tSCIENCE:\t\t\t\t",0);
#endif // _WIN32
    cout << "Known Techs\t\tTier\tField" << endl;
    cout << "-----------\t----\t-----" << endl;
    cout << "" << endl;
    cout << "Cutting Edge Research\tTier\tField" << endl;
    cout << "---------------------\t----\t-----\t" << endl;
    cout << "" << endl;
    cout << "Bleeding Edge Research\tTier\tField" << endl;
    cout << "----------------------\t----\t-----\t" << endl;
    cout << endl;
}

void my_stats(){
    using namespace std;
#ifdef _WIN32
    my_println("\t\t\t\tSTATS:\t\t\t\t",0);
#endif // _WIN32
    cout << "Friendlies killed in action:\t" << 0 << endl;
    cout << "Total damage received:\t\t" << 0 << endl;
    cout << "Total healed:\t\t\t" << 0 << endl;
    cout << "------------------------------" << endl;
    cout << "Xenos killed:\t\t\t" << 0 << endl;
    cout << "Hostile colonists slain:\t" << 0 << endl;
    cout << "Turns:\t\t\t\t" << 0 << endl;
    cout << "Total damage dealt\t\t" << 0 << endl;
    cout << "Most damage dealt in 1 turn:\t" << 0 << endl;
    cout << "Most enemies killed in 1 turn:\t" << 0 << endl;
    cout << "------------------------------" << endl;
    cout << "Total material acquired:\t" << 0 << endl;
    cout << "Total biomass acquired:\t\t" << 0 << endl;
    cout << "Total energy acquired:\t\t" << 0 << endl;
    cout << "Total material spent:\t\t" << 0 << endl;
    cout << "Total biomass spent:\t\t" << 0 << endl;
    cout << "Total energy spent:\t\t" << 0 << endl;
}

void my_activities(){
    using namespace std;
#ifdef _WIN32
    my_println("\t\t\t\tACTIVITIES:\t\t\t\t",0);
#endif // _WIN32
    cout << "Engineer: " << "Commander! I am bored, gimme work!" << endl;
    cout << "Scientist: " << "What we doin'?" << endl;
    cout << "Security: " << "Work, work..." << endl;
    cout << "Labourer: " << "I be doin' that." << endl;
    cout << "Labourer 2: " << "Me busy, leave me alone!" << endl;
    cout << "Labourer 3: " << "We're being attacked!" << endl;
    cout << "Labourer 4: " << "I'm not paid enough for this..." << endl;
    cout << "Labourer 5: " << "Work complete." << endl;
    cout << endl;
}

int process_commands(){
    std::cout << "$ ";
    std::string command;
    std::cin >> command;
    while (true){ // Enter commands
        if      (command == "quit"      || command == "q"){my_exit(); return 0; }
        else if (command == "help"      || command == "?"){ fHelp();}
        else if (command == "look"      || command == "l"){ my_look(world_map, World_Map_X, World_Map_Y); }
        else if (command == "team"      || command == "t") {my_team(); }
        else if (command == "save"      || command == "sv"){ }
        else if (command == "end"       || command == "\t") {break; }
        else if (command == "resources" || command == "r") { my_resources();}
        else if (command == "north"     || command == "n"){ }
        else if (command == "east"      || command == "e"){ }
        else if (command == "south"     || command == "s"){ }
        else if (command == "west"      || command == "w"){ }
        else if (command == "train"     || command == "tr"){ }
        else if (command == "build"     || command == "b") { }
        else if (command == "infrastructure" || command == "i"){my_inf(); }
        else if (command == "science"   || command == "sci"){ my_sci();}
        else if (command == "stats"     || command == "#"){my_stats();}
        else if (command == "activity"  || command == "a"){my_activities();}
        else {
            std::cout << "Invalid command." << std::endl;
        }
        std::cout << "$ ";
        std::cin >> command;
    }
    return 1;
}

// being able to see xenos decisions can be very menacing for players, and intriguing
void process_xenos_decisions(){
    // determine if any xenos evolve (this wastes their turn)
    // are any xenos affected by environmental hazards (such as acid rain)?
    // determine how those xenos respond
    // are any xenos affected by fear hazards (such as fire or explosives)?
    // do any xenos sense the players?
    // determine how those xenos respond
    // do any xenos sense colonists? (this means the xenos prioritise players over colonists... not sure if this is a good idea)
    // determine how those xenos respond
    // determine how the other xenos behave with no stimuli
}

void process_colonist_decisions(){
    // determine goals
    // determine strategy to meet those goals
    // determine policy options
    // assessment of consequences
    // profit maximisation
    // emotional
    // self-aggrandizement
    // archetypes:  warlord, merchant, backstabber, expansionist,
    //              war by proxy, containment, isolationist,
    //              late game dominator, chess strategist, paranoid schizophrenic,
    //

}

void process_biome_changes(){

}

void process_player_improvements(){

}

int main()
{
#ifdef _WIN32
    SetConsoleTitle("Aldinach");

    //COORD c;
    //c.X = 800;
    //c.Y = 600;
    //SMALL_RECT r;
    //r.Left=0, r.Top=0, r.Right=c.X-1, r.Bottom=c.Y-1;
    //HANDLE hConOut = GetStdHandle( STD_OUTPUT_HANDLE );
    //SetConsoleWindowInfo(hConOut, TRUE, &r);
    //SetConsoleScreenBufferSize(hConOut, c);
    //SetConsoleDisplayMode(hConOut, 1);
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions
    MoveWindow(console, 0, 0, 800, 600, TRUE); // 800 width, 100 height
    my_println("\t\t\t\t Welcome to Aldinach \t\t\t\t", 0);

#endif // _WIN32
    print_skull();
    std::cout << "By what name do you wish to be mourned? ";
    std::string name;
    std::cin >> name;
    std::cout << std::endl;
    std::cout   << "What difficulty would you like?" << std::endl
                << "[1] I'm a grub." << std::endl
                << "[2] Come Get Some." << std::endl
                << "[3] No Mercy." << std::endl;
    unsigned difficulty_choice;
    std::cin >> difficulty_choice; // entering a letter causes an infinite loop
    if(difficulty_choice != 3){std::cout << "Your request to have an easy life has been denied. We shall show no mercy, grub." << std::endl;}
    else {std::cout << "Your bravery has been noted. We shall show no mercy." << std::endl << std::endl << std::endl;}

    std::cout << "Press any key to continue...";
    std::string tmp;
    std::cin >> tmp;
    display_prologue();
    while(true){ // Each turn
        int ret = process_commands(); // Player makes the first move
        if(ret == 0){return 0;} // Lets player determine when to quit
        process_xenos_decisions(); // Xenos get first turn advantage on the Colonists
        process_colonist_decisions(); // Colonists respond to xenos decisions
        process_biome_changes(); // Biome variation can negatively impact players next turn, and impact improvements unexpectedly
        process_player_improvements(); // Forces players to take into account hostile actions when it comes to their improvements
    }
}
