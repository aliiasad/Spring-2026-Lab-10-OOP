# include <iostream>
using namespace std;

int ln (const char*);
char* cpy(const char*);

class Gadget    {
    private:
        int code;
        char* name;
        bool isActive;
        static int gadgetCount; // destruction logic
        static int robotCount; // track robos to destroy (-10)
    public:
        Gadget();
        Gadget(char*, int, bool);
        Gadget(const Gadget&);
        Gadget(Gadget&&);
        ~Gadget();

        static void registerRobot();
        static void adjustCount(int);
    
        static int getGadgetCount();
        inline const char* getName() const;
        inline int getCode() const;
        inline bool getActive() const;

        Gadget& setCode(int);
        Gadget& setName(const char*);
        Gadget& setActive(bool);

        void display() const;
};

int Gadget :: gadgetCount = 0;
int Gadget :: robotCount = 0;

void Gadget :: registerRobot()  { 
    robotCount++; 
}

void Gadget :: adjustCount(int n) { 
    gadgetCount += n; 
}

int Gadget :: getGadgetCount() { 
        return gadgetCount - (robotCount * 10);
    }

Gadget :: Gadget()  {
    this->name = cpy("Unknown");
    this->code = 0;
    this->isActive = false;
    gadgetCount++;
}

Gadget :: Gadget(char* name, int code, bool isActive)  {
    this->name = cpy(name);
    this->code = code;
    this->isActive = isActive;
    gadgetCount++;
}

Gadget :: Gadget(const Gadget& dummy)   {
    this->name = cpy(dummy.name);
    this->code = dummy.code;
    this->isActive = dummy.isActive;
    gadgetCount++;
}

Gadget :: Gadget(Gadget&& dummy)    {
    this->name = dummy.name;
    dummy.name = nullptr;

    this->code = dummy.code;
    dummy.code = 0;

    this->isActive = dummy.isActive;
    dummy.isActive = 0;
}

Gadget& Gadget :: setCode(int code) {
    this->code = code;
    return *this;
}

Gadget& Gadget :: setName(const char* name) {
    delete[] this->name;
    this->name = cpy(name);
    return *this;
}

Gadget& Gadget :: setActive(bool isActive) {
    this->isActive = isActive;
    return *this;
}

inline const char* Gadget :: getName() const { return name;}

inline int Gadget :: getCode() const { return code; }

inline bool Gadget :: getActive() const { return isActive; }

void Gadget :: display() const {
    cout << "Gadget ID: " << code << " | Name: " << name 
         << " | Active: " << (isActive ? "Yes" : "No") << endl;
}

Gadget :: ~Gadget() {
    delete[] name;
    name = nullptr;;
    gadgetCount--;
}

class Human {
    private:
        char* name;
        Gadget* gadget;
    public:
        Human();
        Human(const char*, Gadget*);
        ~Human();

        Human& setName(const char*);
        Human& setGadget(Gadget*);

        const char* getName() const;
        Gadget* getGadget() const;

        void display() const;
};

Human :: Human() {
    this->name   = cpy("Unknown");
    this->gadget = nullptr;
}

Human :: Human(const char* name, Gadget* gadget) {
    this->name   = cpy(name);
    this->gadget = gadget;
}

Human :: ~Human() {
    delete[] name;
    name = nullptr;
}

Human& Human :: setName(const char* name) {
    delete[] this->name;
    this->name = cpy(name);
    return *this;
}

Human& Human :: setGadget(Gadget* gadget) {
    this->gadget = gadget;
    return *this;
}

const char* Human :: getName() const { 
    return name;
}

Gadget* Human :: getGadget() const { 
    return gadget;
}

void Human :: display() const {
    cout << "Human: " << name << endl;
    if (gadget) {
        cout << "Borrowed Gadget:" << endl;
        cout << "  ";
        gadget->display();
    } else {
        cout << "  No gadget borrowed." << endl;
    }
}

class VIPHuman {
    private:
        char* name;
        Gadget gadget;   
    public:
        VIPHuman();
        VIPHuman(const char*, const Gadget&);
        ~VIPHuman();

        VIPHuman& setName(const char*);
        VIPHuman& setGadget(const Gadget&);

        const char* getName()  const;
        Gadget& getGadget();

        void display() const;
};

VIPHuman :: VIPHuman() {
    this->name = cpy("Unknown");
}

VIPHuman :: VIPHuman(const char* n, const Gadget& g) : gadget(g)   {
    this->name = cpy(n);
}

VIPHuman& VIPHuman :: setName(const char* name) {
    delete[] this->name;
    this->name = cpy(name);
    return *this;
}


VIPHuman& VIPHuman :: setGadget(const Gadget& g) {
    this->gadget.setCode(g.getCode()).setName  (g.getName()).setActive(g.getActive());
    return *this;
}

const char* VIPHuman :: getName()  const { 
    return name;   
}

Gadget& VIPHuman :: getGadget() { 
    return gadget; 
}

void VIPHuman :: display() const {
    cout << "VIP Human: " << name << endl;
    cout << "Owned Gadget:" << endl;
    gadget.display();
}

VIPHuman :: ~VIPHuman() {
    delete[] name;
    name = nullptr;
}

class Robot {
    private:
        char* name;
        Gadget ownedGadgets[10];
        int ownedCount;
        Gadget* sharedGadgets[5];
        int sharedCount;
    public:
        Robot();
        Robot(const char*);
        ~Robot();

        Robot& setName(const char*);
        Robot& addOwnedGadget (int, const char*, bool);
        Robot& addSharedGadget(Gadget*);

        const char* getName() const;

        void display() const;
};

Robot :: Robot() {
    this->name = cpy("Unknown");
    this->ownedCount  = 0;
    this->sharedCount = 0;
    for (int i = 0; i < 5; i++) 
        sharedGadgets[i] = nullptr;
    Gadget::registerRobot();
}

Robot :: Robot(const char* name) {
    this->name = cpy(name);
    this->ownedCount  = 0;
    this->sharedCount = 0;
    for (int i = 0; i < 5; i++) 
        sharedGadgets[i] = nullptr;
    Gadget::registerRobot();
}

Robot& Robot :: setName(const char* name) {
    delete[] this->name;
    this->name = cpy(name);
    return *this;
}

Robot& Robot :: addOwnedGadget(int code, const char* gName, bool isActive) {
    if (ownedCount < 10) {
        ownedGadgets[ownedCount].setCode(code).setName(gName).setActive(isActive);
        ownedCount++;
    } else {
        cout << "  [Robot " << name << "] Owned gadget limit reached." << endl;
    }
    return *this;
}

Robot& Robot :: addSharedGadget(Gadget* gadget) {
    if (sharedCount < 5) {
        sharedGadgets[sharedCount] = gadget;
        sharedCount++;
    } else {
        cout << "  [Robot " << name << "] Shared gadget limit reached." << endl;
    }
    return *this;
}

const char* Robot :: getName() const { 
    return name;
}

void Robot :: display() const {
    cout << "Robot Name: " << name << endl;

    cout << "Owned Gadgets:" << endl;
    if (ownedCount == 0) {
        cout << "  (none)" << endl;
    } else {
        for (int i = 0; i < ownedCount; i++) {
            cout << "  - ";
            ownedGadgets[i].display();
        }
    }

    cout << "Shared Gadgets:" << endl;
    if (sharedCount == 0) {
        cout << "  (none)" << endl;
    } else {
        for (int i = 0; i < sharedCount; i++) {
            cout << "  - ";
            sharedGadgets[i]->display();
        }
    }
}

Robot :: ~Robot() {
    delete[] name; // not deleting shard or owned because shared is aggregation and owned is composition..
    name = nullptr;
}

int main()  {
    int n;
    cout << "How many gadgets would you like to create? ";
    cin >> n;

    Gadget* gadgets = new Gadget[n];

    for (int i = 0; i < n; i++) {
        int code;
        char name[100];
        bool isActive;
        int activeInput;

        cout << "\nGadget " << (i + 1) << ":" << endl;
        cout << "  Enter code: ";
        cin >> code;
        cout << "  Enter name: ";
        cin >> name;
        cout << "  Is active? (1 = Yes, 0 = No): ";
        cin >> activeInput;
        isActive = activeInput;

        gadgets[i].setCode(code).setName(name).setActive(isActive);
    }

    cout << "\n--- All Gadgets ---" << endl;
    for (int i = 0; i < n; i++) gadgets[i].display();
    cout << endl;

    Human shizuka("Shizuka", &gadgets[0]);
    Human gian("Gian",       &gadgets[n - 1]);

    cout << "--- Humans ---" << endl;
    shizuka.display();
    cout << endl;
    gian.display();
    cout << endl;

    VIPHuman nobita("Nobita", gadgets[1 % n]);

    cout << "--- VIP Human ---" << endl;
    nobita.display();
    cout << endl;

    // Step 7: Create robots, add owned and shared gadgets
    Robot doraemon("Doraemon");
    doraemon.addOwnedGadget(301, "Translator Tool", true)
            .addOwnedGadget(302, "Time Freezer",    false)
            .addSharedGadget(&gadgets[0])
            .addSharedGadget(&gadgets[n - 1]);

    Robot dorami("Dorami");
    dorami.addOwnedGadget(401, "Moshi Moshi Hat", true)
          .addSharedGadget(&gadgets[1 % n]);

    cout << "--- Robots ---" << endl;
    doraemon.display();
    cout << endl;
    dorami.display();
    cout << endl;

    cout << "Total Gadgets Created: " << Gadget::getGadgetCount() << endl;
    cout << endl;

    delete[] gadgets;

    return 0;
}

int ln (const char* str)    {
    int i = 0;
    while (str[i] != '\0')  i++;
    return i;
}

char* cpy(const char* str)  {
    char* dest = new char [ln(str) + 1];
    int i = 0;
    while (str[i] != '\0')  {
        dest[i] = str[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}