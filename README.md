# Deliberative agents
## 2<sup>nd</sup> practice of *Artificial Intelligence*, 2022/2023

![image](https://github.com/Mayhem929/practica2/assets/76180710/544a850f-1e2b-4ac8-8f3d-d132505e4885)


This practice consists of the design and implementation of a reactive/deliberative agent capable of perceiving the environment and acting considering a representation of the consequences of its actions and following a search process.

We take as a starting point the world of graphic adventure computer games to try to build virtual characters that exhibit their own intelligent behaviors within the game. We aim to address a common problem in computer game development and we will play around with designing characters that interact autonomously using reactive/deliberative agents.

In our game there are two agents, a player, which we control and can move in 4 directions, and a sleepwalker, which can move in 8 directions and can only be controlled when close to the player.

The practice is structured into 4 levels and a tutorial. In each level we need to develop a certain search algorithm to accomplish a goal.

---

### Level 0 (Tutorial)

**Find the path with the minimum number of actions that leads the PLAYER agent to a target cell.**

In this level we were required to implement a breadth-first search algorithm to find the solution with the least number of actions.

---

### Level 1

**Find the path with the minimum number of actions that leads the SLEEPWALKER agent to a target cell.**

In this level, again, we were required to implement a breadth-first search algorithm but this time the search space was bigger due to there being more possible actions.

---

### Level 2

**Find the path with the minimum battery consumption using the DIJKSTRA'S ALGORITHM (uniform cost) that leads the PLAYER agent to a target cell.**

This time, our algorithm had to be optimal in battery use, so we implemented Dijkstra's algorithm to find the path with the least battery usage.

---

### Level 3

**Find the path with the minimum battery consumption using the A\* ALGORITHM that leads the SLEEPWALKER agent to a target cell.**

Similarly, in this level we needed to go through a bigger search space so implementing a faster algorithm than Dijkstra's was necessary.

---

### Level 4

**Challenge**

In this final level at the start the map or even the position of the agents is unknown, and the goal is to get the most objectives possible before the battery runs out. To carry out this level succesfully it was necesary to mix techniques from both reactive and deliberative agents, for instance, recalculating the current plan was crucial. Moreover, other agents were introduced; villagers, who could obstruct the player or the sleepwalker, and wolves, which apart from this, could even push the player and make him lose track of his current position.

---

## Credits
The base sofware was made by the AI department in UGR. ugr-ccia-IA 
I, Adrián Jaén Fuentes, implemented the files jugador.cpp and jugador.hpp.
Also, big thanks to my teachers Nuria, who helped me helped me through the practice and Juanlu, who developed a telegram bot for the students to test their code. 


