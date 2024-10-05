//
// Created by tillw on 05/10/2024.
//

#ifndef STUFF_H
#define STUFF_H

enum dir {UP, DOWN};

bool login(std::string &password);
void loadWishes(std::vector<std::string> &wishes);
void addWish(std::string &wish);
void removeWish(int index);
void moveWish(int index, dir direction);

#endif //STUFF_H
