#include <iostream> 
#include <chrono>
#include <thread> 
#include <string>
#include <regex>
#include <random>
#include <map>
#include <vector>

class Table 
{
public:
    Table(int padding = 1, 
            const char h_line = '_',
            const char v_line = '|',
            const char border = '*',
            const char header_line = '=')
        :    
        m_PaddingLength(padding), 
        m_HLineSymbol(h_line),
        m_VLineSymbol(v_line),
        m_BorderSymbol(border),
        m_HeaderSymbol(header_line)
    {
    };
        

    template<typename... Args> 
    void AddColumns(Args... args)
    { 
        (AddColumn(args), ...);
    };

    void AddColumn(const std::string& name)
    {
        if (!ColumnData.count(name))
            ColumnData[name];
    };

    void AddValuesToColumn(const std::string& name, const std::vector<std::string>& values)
    {
        ColumnData[name] = values;
    };

    void Print() 
    {
        CalculateTableLength();
        const std::string border = GetCharLine(m_BorderSymbol);
        const std::string hline = GetCharLine(m_HLineSymbol);
        const std::string dummy = GetCharLine(' ');

        /*
        std::cout << 
            border << '\n' << 
            dummy  << '\n' << 
            header << '\n' << 
            dummy  << '\n' << 
            hline  << '\n' << 
            dummy  << '\n' << 
            hline  << '\n' << 
            dummy  << '\n' << 
            hline  << '\n' << 
            dummy  << '\n' << 
            border << '\n';
            */

        std::cout << border << '\n';
        PrintHeader();
        //PrintBody()
        std::cout << border << '\n';
    };

private: 
    void PrintHeader()
    {
        const std::string header = GetCharLine(m_HeaderSymbol);
        const std::string padding = GetCharLine(' ', m_PaddingLength);
        std::cout << '|';
        for (const auto& [colName, colValues] : ColumnData)
        {
            const std::string filler = GetCharLine(' ', m_MaxColumnLength - colName.length());
            std::cout << padding << colName << filler << padding << '|';
        };
        std::cout << '\n' << header << '\n';
    };
    void CalculateTableLength() 
    {
        for (const auto& [colName, colValues] : ColumnData)
        {
            int colLength  = colName.length();
            for (const auto& value : colValues)
            { 
                int valLength = value.length();
                if (valLength > colLength)
                    colLength = valLength;
            };
            if (colLength > m_MaxColumnLength)
                m_MaxColumnLength = colLength;
        };
        m_TableLength = (m_MaxColumnLength + 2 * m_PaddingLength + 1) * ColumnData.size() + 1;
    };

    const std::string GetCharLine(const char symbol, int length)
    {
        return std::string(length, symbol);
    };
    const std::string GetCharLine(const char symbol)
    {
        return std::string(m_TableLength, symbol);
    };


    std::map<std::string, std::vector<std::string>> ColumnData; 
    int m_PaddingLength;
    char m_HLineSymbol;
    char m_VLineSymbol;
    char m_BorderSymbol;
    char m_HeaderSymbol;
    int m_TableLength = 1;
    int m_MaxColumnLength = 0;
    
};

class Player { 
public:  
    Player() 
        : m_Name("Default"), m_Symbol('0') {};
        
    Player(char symbol, std::string name = "Default") 
        : m_Name(name), m_Symbol(symbol) 
    {
        //PrintPlayer();
    };

    void PrintPlayer() 
    {
            std::cout << "NAME: " << m_Name << "\t\t SYMBOL: " << m_Symbol << std::endl;
    };

    const char GetSymbol()
    { 
        return m_Symbol; 
    };
        
    const std::string& GetName()
    { 
        return m_Name; 
    };

    float GetPoints() 
    {
        return m_Points;
    };

    void IncrementWinPoints()
    {
        m_Points++;
    };
        
    void IncrementTiePoints()
    {
        m_Points += 0.5f;
    };

private: 
    std::string m_Name;
    char m_Symbol;
    float m_Points = 0.0f;

};

class Game { 
public:
    Game() 
    {
        Start();
        PrintBoard();
        SetupComputer();
    };

    void SetupComputer()
    {
        std::random_device dev;
        rng = std::mt19937(dev());
        dist9 = std::uniform_int_distribution<std::mt19937::result_type>(0, 8);
    };

    void Start()
    {
        PrintStartMessage();
        WaitMiliSeconds(2); 
        QueryPlayers();
        std::cout << "********************************************************************\n"
                    "\t\tBEGIN GAME!!! \n"
                    "********************************************************************\n";
        
    };

    bool IsRunning() 
    { 
        return m_Running;
    };

    void Turn() 
    {
        QueryPlay();
        ComputerPlay();
        PrintBoard();
        if (CheckResult())
        {
            PrintPlayers();
            ClearBoard();
            if (!Decision("Play again?"))
                m_Running = false;
            else 
                PrintBoard();
        };

    };

private:
    void PrintStartMessage() 
    {
        std::cout << "********************************************************************\n"
                    "\t\tWelcome to TICTACTOE!!! \n"
                    "********************************************************************\n"
                    "RULES:\n"
                    "\t - Players are P1 (you) and P0 (computer).\n"
                    "\t - Each turn a player can place their respective symbol (tic or tac)\n"
                    "\t - First player to draw three symbols in a straight line (veritcal, horizontal or diagonal) wins the game.\n\n";
    };

    void WaitMiliSeconds(int miliseconds) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
    };

    void QueryPlayers() 
    {
        if (Decision("Begin game? (y/n)"))
        {
           
            std::string p0Name;
            std::cout << "Enter your player name: ";
            std::cin >> p0Name;

            char p0Symbol;  
            std::cout << p0Name << " choose your symbol (X or O): ";
            std::cin >> p0Symbol;
            std::cout << '\n';

            p0 = Player(p0Symbol, p0Name);
            p1 = Player(p0.GetSymbol() == 'X' ? '0' : 'X', "Computer");
        }
        else
        {
            m_Running = false;
            return;
        };
     
    };

    void PrintPlayers() 
    {
        //cout p0 and p1 name length
        //make shure the shortest one "completes" the rest of the characters with its spaces
        //print both to form the columns
        

        Table table;
        table.AddColumns("PLAYERS", "SYMBOL", "POINTS");
        table.Print();
    };

    void PrintBoard()
    {
        std::cout << 
                "\n   |   |   \n " <<
                spots[0] << " | " << spots[1] << " | " << spots[2] << " \n"
                "___|___|___\n" 
                "   |   |   \n " <<
                spots[3] << " | " << spots[4] << " | " << spots[5] << " \n"
                "___|___|___\n" 
                "   |   |   \n " <<
                spots[6] << " | " << spots[7] << " | " << spots[8] << " \n"
                "   |   |   \n";
    };

    bool Decision(const std::string& questionString)
    {
        bool valid = true;
        do  
        {
            char answer;
            std::cout << questionString << "\t"; 
            std::cin.get(answer);

            if (answer == 'y')
                return true;
            else if (answer == 'n')
                return false;
            else 
            {
                std::cout << "Invalid answer!" << std::endl;
                valid = false;
            };
        }
        while (!valid);

        std::cout << "Something went wrong" << std::endl;
        return false;
    };

    void QueryPlay() 
    {
        bool valid = true;

        std::regex numberRegex("[0-9]*");

        do
        {

            std::string spotString;
            std::cout << "Choose a spot to place an '" << p0.GetSymbol() << "' (1 - 9): ";
            std::cin >> spotString;

            if (!std::regex_match(spotString, numberRegex))
            {
                std::cout << "Invalid answer! Must enter a number." << std::endl;
                valid = false;
                continue;
            };

            int spot = std::stoi(spotString) - 1;
            
            if (spot < 0 || spot > 8)
            {
                std::cout << "Invalid answer! Spots are numbered from 1 to 9." << std::endl;
                valid = false;
            }
            else if (spots[spot] != ' ')
            {
                std::cout << "Invalid answer! There is allready an '" << spots[spot] << "' there!\nChoose somewhere else." << std::endl;
                valid = false;
            }
            else 
            {
                spots[spot] = p0.GetSymbol();
                valid = true;
            };
        }
        while (!valid);
    };

    void ComputerPlay()
    {
        bool valid = false;
        int computerIndex;

        while (spots[computerIndex = dist9(rng)] != ' ');

        spots[computerIndex] = p1.GetSymbol();
    };

    bool CheckResult()
    {
        for (int i = 0; i < 3; i++)
        {
            if (spots[i] != ' ' && spots[i] == spots[i + 1] && spots[i] == spots[i + 2])
            {
                UpdateScore(spots[i]);
                return true;
            };
        };

        for (int i = 0; i < 3; i++)
        {
            if (spots[i] != ' ' && spots[i] == spots[i + 3] && spots[i] == spots[i + 6])
            {
                UpdateScore(spots[i]);
                return true;
            };
        };

        if (spots[4] != ' ' && ((spots[0] == spots[4] && spots[0] == spots[8])
                || spots[2] == spots[4] && spots[2] == spots[6]))
        {
            UpdateScore(spots[4]);
            return true;
        };

        int emptyCount = 0;
        for (int i = 0; i < 9; i++)
        {
            if (spots[i] == ' ')
                emptyCount++;
        };

        if (emptyCount < 3)
        {
            UpdateScore(' ');
            return true;
        };
        return false;
    };

    void UpdateScore(const char playerSymbol)
    { 
        if (playerSymbol == p0.GetSymbol())
        {
            p0.IncrementWinPoints();
            std::cout << p0.GetName() << " wins!\n";
        }
        else if (playerSymbol == p1.GetSymbol())
        {
            p1.IncrementWinPoints();
            std::cout << p1.GetName() << " wins!\n";
        }
        else if (playerSymbol == ' ')
        {
            p0.IncrementTiePoints();
            p1.IncrementTiePoints();
            std::cout << "Tie!" << std::endl;
        };
    };    

    void ClearBoard()
    {
        for (int i = 0; i < 9; i++)
            spots[i] = ' ';
    };

    bool m_Running = true;
    Player p0;
    Player p1;
    char spots[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
    std::uniform_int_distribution<std::mt19937::result_type> dist9;
    std::mt19937 rng;
}; 

int main() { 
    Game game;

    while (game.IsRunning()) 
        game.Turn();
};
