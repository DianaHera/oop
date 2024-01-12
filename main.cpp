#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

// Forward declaration pentru a evita erorile de compilare
template <typename T>
class DrinkingGame;

// Interfata pentru Observer
template <typename T>
class GameObserver {
public:
    virtual void notify(const DrinkingGame<T>& game) = 0;
    virtual ~GameObserver() = default;
};

// Clasa concreta a Observer-ului
template <typename T>
class ConcreteGameObserver : public GameObserver<T> {
public:
    void notify(const DrinkingGame<T>& game) override;
};

// Clasa Player este acum o clasa sablon
template <typename T>
class Players {
public:
    Players(const T& get_name) : name(get_name) {}

    const T& getName() const {
        return name;
    }

protected:
    T name;
};

// Clasa concreta care implementeaza Factory Method
template <typename T>
class ConcretePlayerFactory;  // Forward declaration pentru a evita erorile de compilare

template <typename T>
class PlayerFactory {
public:
    virtual Players<T>* createPlayer(const T& name) const = 0;
    virtual ~PlayerFactory() = default;
};

// Clasa Rules este acum o clasa sablon
template <typename T>
class Rules {
public:
    Rules(const T& get_rule) : rule(get_rule) {}

    const T& getRule() const {
        return rule;
    }

protected:
    T rule;
};

// Clasa subject care utilizeaza Observer
template <typename T>
class DrinkingGame {
public:
    DrinkingGame(const std::vector<Players<T>*>& players, const std::vector<Rules<T>*>& rules) {
        if (!players.empty()) {
            int random_player = rand() % players.size();
            name = players[random_player]->getName();
        }

        if (!rules.empty()) {
            int random_rule = rand() % rules.size();
            rule = rules[random_rule]->getRule();
        }
    }

    void write() const {  // Adăugați const aici
        std::cout << "Nume: " << name << "\n";
        std::cout << "Regula: " << rule << "\n";
    }

    // Adăugare observator
    void addObserver(GameObserver<T>* observer) {
        observers.push_back(observer);
    }

    // Notificare observatori
    void notifyObservers() {
        for (GameObserver<T>* observer : observers) {
            observer->notify(*this);
        }
    }

    ~DrinkingGame() = default;

private:
    T name;
    T rule;

    // Lista de observatori
    std::vector<GameObserver<T>*> observers;
};

// Implementarea functiei notify pentru ConcreteGameObserver
template <typename T>
void ConcreteGameObserver<T>::notify(const DrinkingGame<T>& game) {
    std::cout << "Observer notificat cu starea jocului.\n";
    game.write();
}

// Clasa concreta care implementeaza Factory Method
template <typename T>
class ConcretePlayerFactory : public PlayerFactory<T> {
public:
    Players<T>* createPlayer(const T& name) const override {
        return new Players<T>(name);
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Utilizarea Factory Method pentru crearea jucatorilor
    PlayerFactory<std::string>* playerFactory = new ConcretePlayerFactory<std::string>();
    std::vector<Players<std::string>*> players;
    players.push_back(playerFactory->createPlayer("Diana"));
    players.push_back(playerFactory->createPlayer("Ana"));
    players.push_back(playerFactory->createPlayer("Andreea"));

    // Crearea vectorului rules unde sunt inserate regulile jocului
    std::vector<Rules<std::string>*> rules;
    rules.push_back(new Rules<std::string>("Povesteste ultima ta intalnire sau bea de 3 ori"));
    rules.push_back(new Rules<std::string>("Da mail unui profesor și spune-i ca ai 14 warninguri sau termina-ți bautura"));
    rules.push_back(new Rules<std::string>("Cata hartie igienica ai folosit cand te-ai dus la baie, de atatea ori trebuie sa bei"));
    rules.push_back(new Rules<std::string>("Termina proiectul la OOP inainte de termen sau ai restanta"));
    rules.push_back(new Rules<std::string>("Da-i Fuck, Marry, Kill jucatorului din stanga"));

    // Utilizarea Observer pentru notificarea evenimentelor in DrinkingGame
    GameObserver<std::string>* observer = new ConcreteGameObserver<std::string>();
    DrinkingGame<std::string> game(players, rules);
    game.addObserver(observer);

    // Rulare si notificare observator
    game.write();
    game.notifyObservers();

    // Eliberarea memoriei
    delete playerFactory;

    for (Players<std::string>* player : players) {
        delete player;
    }

    for (Rules<std::string>* rule : rules) {
        delete rule;
    }

    delete observer;

    return 0;
}
