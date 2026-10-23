#include <iostream>
#include <string>
using namespace std;

class HeroNode {
public:
    int gold, height, size;
    string name;
    HeroNode *left, *right;
    HeroNode(int g, string n) : gold(g), name(n), height(1), size(1), left(nullptr), right(nullptr) {}
};

class Guild {
private:
    HeroNode* root;

    int getHeight(HeroNode* n) { return n ? n->height : 0; }
    int getSize(HeroNode* n) { return n ? n->size : 0; }
    int getBalance(HeroNode* n) { return n ? getHeight(n->left) - getHeight(n->right) : 0; }

    void update(HeroNode* n) {
        int l = getHeight(n->left), r = getHeight(n->right);
        n->height = 1 + (l > r ? l : r);
        n->size = 1 + getSize(n->left) + getSize(n->right);
    }

    HeroNode* rotateRight(HeroNode* y) {
        HeroNode* x = y->left;
        y->left = x->right; x->right = y;
        update(y); update(x);
        return x;
    }

    HeroNode* rotateLeft(HeroNode* x) {
        HeroNode* y = x->right;
        x->right = y->left; y->left = x;
        update(x); update(y);
        return y;
    }

    HeroNode* rebalance(HeroNode* n) {
        update(n);
        int b = getBalance(n);
        if (b > 1) {
            if (getBalance(n->left) < 0) n->left = rotateLeft(n->left);
            return rotateRight(n);
        }
        if (b < -1) {
            if (getBalance(n->right) > 0) n->right = rotateRight(n->right);
            return rotateLeft(n);
        }
        return n;
    }

    HeroNode* addNode(HeroNode* n, int g, string name) {
        if (!n) return new HeroNode(g, name);
        if (g < n->gold) n->left = addNode(n->left, g, name);
        else if (g > n->gold) n->right = addNode(n->right, g, name);
        else return n;
        return rebalance(n);
    }

    HeroNode* removeNode(HeroNode* n, int g) {
        if (!n) return nullptr;
        if (g < n->gold) n->left = removeNode(n->left, g);
        else if (g > n->gold) n->right = removeNode(n->right, g);
        else {
            if (!n->left || !n->right) {
                HeroNode* child = n->left ? n->left : n->right;
                delete n;
                return child;
            }
            HeroNode* s = n->right;
            while (s->left) s = s->left;
            n->gold = s->gold; n->name = s->name;
            n->right = removeNode(n->right, s->gold);
        }
        return rebalance(n);
    }

    bool checkBalanced(HeroNode* n) {
        if (!n) return true;
        int b = getBalance(n);
        return b >= -1 && b <= 1 && checkBalanced(n->left) && checkBalanced(n->right);
    }

    void destroy(HeroNode* n) {
        if (!n) return;
        destroy(n->left); destroy(n->right); delete n;
    }

public:
    Guild() : root(nullptr) {}
    ~Guild() { destroy(root); }

    void addHero(int g, string n) { root = addNode(root, g, n); }
    void removeHero(int g) { root = removeNode(root, g); }

    HeroNode* richest() {
        HeroNode* n = root;
        if (!n) return nullptr;
        while (n->right) n = n->right;
        return n;
    }

    HeroNode* poorest() {
        HeroNode* n = root;
        if (!n) return nullptr;
        while (n->left) n = n->left;
        return n;
    }

    int countRicherThan(int g) {
        int count = 0;
        HeroNode* n = root;
        while (n) {
            if (g < n->gold) {
                count += 1 + getSize(n->right);
                n = n->left;
            } else n = n->right;
        }
        return count;
    }

    HeroNode* kthRichest(int k) {
        if (k < 1 || k > getSize(root)) return nullptr;
        HeroNode* n = root;
        while (n) {
            int r = getSize(n->right);
            if (k == r + 1) return n;
            if (k <= r) n = n->right;
            else { k -= r + 1; n = n->left; }
        }
        return nullptr;
    }

    void changeGold(int oldGold, int newGold) {
        HeroNode* n = root;
        while (n && n->gold != oldGold)
            n = oldGold < n->gold ? n->left : n->right;
        if (!n || oldGold == newGold) return;

        HeroNode* check = root;
        while (check) {
            if (check->gold == newGold) return;
            check = newGold < check->gold ? check->left : check->right;
        }
        string name = n->name;
        removeHero(oldGold);
        addHero(newGold, name);
    }

    int height() { return getHeight(root); }
    int size() { return getSize(root); }
    bool isBalanced() { return checkBalanced(root); }
};

int main() {
    Guild guild;
    guild.addHero(500, "Aria"); guild.addHero(120, "Borin");
    guild.addHero(900, "Cael"); guild.addHero(300, "Dara");
    guild.addHero(750, "Elric"); guild.addHero(50, "Fenna");
    guild.addHero(640, "Gorm"); guild.addHero(220, "Hilda");

    cout << "Richest: " << guild.richest()->name << " (" << guild.richest()->gold << ")" << endl;
    cout << "Poorest: " << guild.poorest()->name << " (" << guild.poorest()->gold << ")" << endl;
    cout << "Heroes richer than 300 gold: " << guild.countRicherThan(300) << endl;
    cout << "Heroes richer than 1000 gold: " << guild.countRicherThan(1000) << endl;
    cout << "3rd richest: " << guild.kthRichest(3)->name << " (" << guild.kthRichest(3)->gold << ")" << endl;

    cout << endl << "Fenna finds a treasure: 50 -> 1000 gold" << endl;
    guild.changeGold(50, 1000);
    cout << "Richest: " << guild.richest()->name << " (" << guild.richest()->gold << ")" << endl;
    cout << "Poorest: " << guild.poorest()->name << " (" << guild.poorest()->gold << ")" << endl;
    cout << "Heroes richer than 300 gold: " << guild.countRicherThan(300) << endl;
    cout << "Heroes: " << guild.size() << ", height: " << guild.height()
         << ", balanced: " << (guild.isBalanced() ? "yes" : "NO") << endl;
    return 0;
}