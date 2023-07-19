# Reactive/Deliberative agents
## My personal solution to the 2<sup>nd</sup> practice of *Artificial Intelligence*, 2022/2023

![image](https://github.com/Mayhem929/ReactiveDeliberativeAgents/assets/76180710/ed0eda4c-2914-456b-8171-911f3c350dbf)


This practice consists of the design and implementation of a reactive/deliberative agent capable of perceiving the environment and acting considering a representation of the consequences of its actions and following a search process.

We take as a starting point the world of graphic adventure computer games to try to build virtual characters that exhibit their own intelligent behaviors within the game. We aim to address a common problem in computer game development and we will play around with designing characters that interact autonomously using reactive/deliberative agents.

In our game there are two agents, a player, which we control and can move in 4 directions, and a sleepwalker, which can move in 8 directions and can only be controlled when close to the player.

The practice is structured into 4 levels and a tutorial. In each level we need to develop a certain search algorithm to accomplish a goal.

---

### Level 0 (Tutorial)

**Find the path with the minimum number of actions that leads the PLAYER agent to a target cell.**

In this level we were required to implement a breadth-first search algorithm to find the solution with the least number of actions.


### Level 1

**Find the path with the minimum number of actions that leads the SLEEPWALKER agent to a target cell.**

In this level, again, we were required to implement a breadth-first search algorithm but this time the search space was bigger due to there being more possible actions.


### Level 2

**Find the path with the minimum battery consumption using the DIJKSTRA'S ALGORITHM (uniform cost) that leads the PLAYER agent to a target cell.**

This time, our algorithm had to be optimal in battery use, so we implemented Dijkstra's algorithm to find the path with the least battery usage.


### Level 3

**Find the path with the minimum battery consumption using the A\* ALGORITHM that leads the SLEEPWALKER agent to a target cell.**

Similarly, in this level we needed to go through a bigger search space so implementing a faster algorithm than Dijkstra's was necessary.


### Level 4

**Challenge**

In this final level at the start the map or even the position of the agents is unknown, and the goal is to get the most objectives possible before the battery runs out. To carry out this level succesfully it was necesary to mix techniques from both reactive and deliberative agents, for instance, recalculating the current plan was crucial. Moreover, other agents were introduced; villagers, who could obstruct the player or the sleepwalker, and wolves, which apart from this, could even push the player and make him lose track of his current position.

---

## Ranking

The teachers made a [telegram bot](https://github.com/jlsuarezdiaz/IA-BOT-UGR-23) in order to test our AIs on some test maps, and would give points based on how many levels it passed from 1 to 3, and for level 4 based on how many objectives our AI could accomplish in each test map. They even made a leaderboard so that we could see how other students were doing. In the end, my AI got first place among all the students that took part in the competition and I got awarded some cool badges :D


![image](https://github.com/Mayhem929/ReactiveDeliberativeAgents/assets/76180710/03813056-f631-41ec-97c9-ed10054c0a9a) ![image](https://github.com/Mayhem929/ReactiveDeliberativeAgents/assets/76180710/37a5b655-823a-4d44-9cda-2861db873f14) ![image](https://github.com/Mayhem929/ReactiveDeliberativeAgents/assets/76180710/8664948f-7f86-4579-9249-494215b2e6a4)


---

## Credits
The base sofware was made by the AI department in UGR. [ugr-ccia-IA](https://github.com/ugr-ccia-IA/practica2).

I, Adrián Jaén Fuentes, implemented the files jugador.cpp and jugador.hpp.

Also, big thanks to my teachers [@rbnuria](https://github.com/rbnuria), who helped me helped me through the practice and [@jlsuarezdiaz](https://github.com/jlsuarezdiaz), who developed the telegram bot for the students to test their code. 


