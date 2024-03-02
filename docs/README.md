---
layout: home
permalink: index.html

# Please update this with your repository name and title
repository-name: e18-4yp-PeraSwarm-Simultaneous-Localization-and-Mapping-in-Mixed-Reality-Environment
title: PeraSwarm Simultaneous Localization and Mapping in Mixed Reality Environment
---

[comment]: # "This is the standard layout for the project, but you can clean this and use your own template"

# PeraSwarm: Simultaneous Localization and Mapping in Mixed Reality Environment

#### Team

- E/18/077, N.S. Dharmarathne, [email](mailto:e18077@eng.pdn.ac.lk)
- E/18/150, H.M.Y.S. Jayarathna, [email](mailto:e18150@eng.pdn.ac.lk)
- E/18/227, D.H. Mudalige, [email](mailto:e18227@eng.pdn.ac.lk)

#### Supervisors

- Prof. Roshan G. Ragel, [email](mailto:roshanr@eng.pdn.ac.lk)
- Dr. Isuru Nawinne, [email](mailto:isurunawinne@eng.pdn.ac.lk)
- Mr. Nuwan Jaliyagoda, [email](mailto:nuwanjaliyagoda@eng.pdn.ac.lk)

#### Table of content

1. [Abstract](#abstract)
2. [Introduction](#introduction)
3. [Related works](#related-works)
4. [Experiment Setup and Implementation](#experiment-setup-and-implementation)
5. [Conclusion](#conclusion)
6. [Publications](#publications)
7. [Links](#links)

<!-- 3. [Methodology](#methodology)
4. [Experiment Setup and Implementation](#experiment-setup-and-implementation)
5. [Results and Analysis](#results-and-analysis)
6. [Conclusion](#conclusion)
7. [Publications](#publications)
8. [Links](#links) -->

---

![Pera Swarm](./images/pera_swarm.png)

## Abstract
This paper explores the captivating field of swarm robotics, where multiple robots collaborate to achieve tasks in decentralized and distributed manner. Inspired by natural swarm behaviors, such as those observed in ants and bees, swarm robotics aims to analyze emergent behaviors within robot swarms, including aggregation, dispersion, and collective movement. The focus here is on the exploration behavior of swarm robotics in unknown environments. While swarm robotics offers increased efficiency, reliability, and cost-effectiveness, implementing distributed exploration poses challenges in coordination, communication, decision-making, and fault tolerance. Nonetheless, advancements in robotics and artificial intelligence pave the way for overcoming these challenges, leading to more efficient and robust exploration missions.

Index Terms — Swarm robotics, Collaborative mapping, Swarm intelligence

## Introduction
Swarm robotics is a fascinating field of robotics that involves multiple robots working together to achieve tasks in a decentralized and distributed manner. It is inspired by the collective behavior observed in natural swarms, such as colonies of ants and hives of bees, where individual entities work together towards a common goal. 

<img src="images/ant_swarm.jpg" alt="Fig. Ant Swarm" height="250"/> <img src="images/bee_swarm.gif" alt="Fig. Bee Swarm" height="250"/>
<p class="caption text-center">Fig. Natural swarms (Ants, Bees)</p>

Swarm robotics mainly focuses on the collective behaviors of robots working together without centralized control, particularly in exploration tasks within unknown environments. These types of autonomous robots are mostly used in real-world applications such as search and rescue missions, in exploration and surveillance systems and intrusion detection or fault identification systems in complex environments. They are also used in hazardous environment navigations, reaching areas that are inaccessible or too dangerous for humans. Efficient mapping strategies are crucial for these robots to navigate and adapt to changes. Swarms of robots offer advantages over single robots, including increased efficiency, reliability, and cost-effectiveness due to collaborative efforts, redundancy, and flexibility, making them ideal for time-sensitive missions and unpredictable conditions.

<img src="images/mit_swarm.png" alt="Fig. Ant Swarm" height="250"/>
<p class="caption text-center">Fig. MIT Multirobot Mapping Sets</p>
<!-- <a href="https://spectrum.ieee.org/multi-robot-slam-icra2023">Visit</a> -->

<img src="images/harvard_swarm.gif" alt="Fig. Bee Swarm" height="250"/>
<p class="caption text-center">Fig. Harvard Kilobots</p>
<!-- <a href="https://spectrum.ieee.org/a-thousand-kilobots-self-assemble">Visit</a> -->


## Related works
In the past few decades, there’s been a surge of interest in searching through different potentials of swarm robotics. In this session, we’ll take a deep dive into existing research to gain a clear picture of where this field stands today. Here, our major goal is to pinpoint areas that need more attention, spot emerging patterns, and highlight important theories.

Rothermich et al. reported about a swarm of simple robots to explore and map an unknown building. The paper discussed the general issues and questions of swarm robotics, such as when and how to use swarms, and how to evaluate their performance. The paper also described the methods and tools used for simulation and experimentation, and the algorithms developed for collaborative localization, task allocation, and mapping.

According to the SwarMap system, a swarm of robots collaboratively built with a shared grid stored in the cloud using Bayesian Filters. These robots exchanged their localization estimates and sent updates to the grid, resulting in enhanced localization and map quality compared to using odometry alone. The experiments also showed that larger swarm groups yield better maps due to improved cooperative localization. Despite, another methodology that allows each robot to update its belief about its position using only local information was formulated by the same set of people. Here, it involved a leader robot equipped with good localization capabilities, which would provide position estimates to the rest of the swarm. This leader robot guided the group by suggesting the direction to be followed, enabling the robots to cooperatively localize themselves using an approximate decentralized algorithm. They also maintained a group cohesion during navigation by collectively estimating their positions. The use of information from immediate neighbors allowed each robot to perform localization effectively and collective motion strategies played a crucial role in achieving accurate and reliable cooperative localization.

Zhang et al. used a fully decentralized approach with an occupancy grid map and four states to represent obstacles, unexplored areas, free spaces, and frontiers. The exploration strategy was based on frontiers, which were the boundaries between explored and unexplored areas. The robots were repeatedly detecting these frontiers and were moving towards them until there were no more frontiers, and consequently, no more unknown regions. At each time step, each robot broadcasted its own position and local map, integrating the information received from other robots into its local map.

<div class="figure container">
  <img class="mx-auto d-block" src="./images/brain-storm.png" alt="Fig. Grid map environment" width="500" />
  <p class="caption text-center">Fig. Grid map environment</p>
</div>
<div class="figure container">
  <img class="mx-auto d-block" src="./images/flow.png" alt="Fig. Overall schematic diagram" width="500" />
  <p class="caption text-center">Fig. Overall schematic diagram</p>
</div>

Abu-Aisheh et al. introduced the Atlas Algorithm, a novel approach for sparse swarm robot exploration and mapping. However, it may not be suitable for decentralized systems, as it was relied on a centralized controller to direct robots in systematic exploration. Atlas would ensure mapping completion even with just a single robot. This was achieved through a systematic exploration controlled by a central controller, which can direct robots to unexplored areas, focusing on frontier cells rather than frontier robots. This approach allowed the frontier to expand away from the starting point, with robots making circular movements to explore. In obstaclerich environments, the swarm can divide into subgroups for efficient navigation.

Atlas was evaluated against algorithms such as Ramaithithima, random walk, and ballistic walk. Here, metrics like exploration time, cost, efficiency, map completeness, and quality were used for comparison. Results demonstrated that, Atlas was superior in exploration speed and map completeness. Also, Atlas was having lower exploration cost and higher efficiency with compared to the alternatives. Overall, while Atlas presents a comprehensive exploration and mapping solution for centralized systems, our search continues for a decentralized approach that aligns with our objectives.

A system with comprehensive explanations on collaborative mapping and navigation algorithms was formulated by Arvanitakis et al. where each robot in the swarm was equipped with a limited field of view, a limited-range finder, and a magnetometer to infer its orientation. The robots were assigned fixed stationary targets. They initially explored towards their target areas and then were guided towards their targets. The robots exchanged their maps in a collaborative manner during the exploration.

Dieter et al. introduced a distributed, multi-robot mapping and exploration approach that can handle unknown initial locations in limited communicating environments. They were able to tackle the challenges of coordinating robots without knowledge of their relative locations, integrating their data into consistent maps. They introduced a decisionbase coordination technique to balance exploration and location verification, along with a Bayesian map merging technique for estimating map overlaps. Here, the robots were exploring unknown areas with verifying hypotheses, using a shared map that updates when robots detect each other.

<div class="figure container">
  <img class="mx-auto d-block" src="./images/BehavioralAlgorithms.png" alt="Fig. Behavioral Algorithms related to Swarm Intelligence" width="600" />
  <p class="caption text-center">Fig. Behavioral Algorithms related to Swarm Intelligence</p>
</div>

Previous approaches mainly relied on random motion models, neglecting prior information in the map but Rogers et al. focused on the challenge of constructing an occupancy grid map in an unknown environment using a swarm of resource-constrained robots with limited sensing capabilities. The proposed strategy introduced a collaborative exploration approach, dividing the swarm into landmark robots and mapper robots, where the former guide the latter to promising areas to collect proximity measurements for map incorporation. The positions of the landmark robots were optimized to maximize new information while adhering to connectivity constraints, effectively decoupling the problem of directing the swarm from the mapping task. Extensive simulated experiments validated the performance, improves exploration efficiency and ensures that new information is maximized, leading to more effective mapping.

<div class="figure container">
  <img class="mx-auto d-block" src="./images/MappingAlgorithms.png" alt="Fig. Some famous mapping algorithms in swarm robotics" width="600" />
  <p class="caption text-center">Fig. Some famous mapping algorithms in swarmrobotics </p>
</div>

In addition to the above explained theories and algorithms and also to get better understanding on these subjects, the Figure 2 shows an overview of mapping algorithms in swarm robotics, categorized in a systematic way. When talking about other technologies related to robotics, Simultaneous Localization and Mapping (SLAM) can be considered as a pivotal element which can be used to create detailed maps of the surrounding while concurrently determining the robot’s position within them. Most of the SLAM based algorithms integrate data through sensor fusion, with diverse ranges such as cameras, LIDARs, or sonars, refining both the map and localization estimates over time. Widely used systems like Google’s Cartographer and ORB-SLAM exemplify the versatility of SLAM in navigating and mapping unknown environments with precision and robustness.

<div class="figure container">
  <img class="mx-auto d-block" src="./images/SLAMapproaches.png" alt="Fig. Simultaneous Localization and Mapping approaches" width="600" />
  <p class="caption text-center">Fig. Simultaneous Localization and Mapping approaches</p>
</div>

Figure 3 shows current approaches in SLAM, like direct implementations of SLAM (specially, theories like odometry and loop closure) and some derivatives from previously mentioned algorithms (for example, occupancy grid approach for mapping inside SLAM). Despite, most of these approaches were primarily focused on non-swarm systems, which are limited in scalability, adaptability. On the other hand, Swarm SLAM can be considered as a promising approach that leverage the decentralized nature of robot swarms to achieve scalable, flexible, and fault-tolerant exploration and mapping. For example, when considering broader applications like in the agricultural domain, SLAM technology has been employed for precision farming, crop monitoring, and autonomous agricultural vehicles.

Collaborative SLAM (C-SLAM) also known as multi-robot SLAM (MRSLAM), a key extension from the foundational principles of swarm SLAM to heterogeneous robot teams which had been used in the recent researches to test selfdriving cars and multi-robot systems. Lajoie et al. survey provides valuable insights into the challenges and opportunities associated with collaborative mapping.

## Experiment Setup and Implementation

Here, we present our progress on developing a multirobot mapping system using occupancy grid approach. We
tested mapping algorithms with virtual robots in our simulated
environment.
Each robot is constrained to move one step at a time in four
directions: North, East, South and West. These limitations play
a crucial role in our multi-robot mapping system, affecting
their exploration patterns and map coverage efficiency. Fig. 1
illustrates the restricted movement capabilities of our robots.

<div class="figure container">
  <img class="mx-auto d-block" src="./images/fig1.png" alt="Fig. Robot movement constraints" width="300" />
  <p class="caption text-center">Fig. Robot movement constraints</p>
</div>

We assume the robots operate in a planer workspace. To
represent the overall environment, we use a grid map. This
map is stored as a matrix, where each element corresponds
to a specific state within the environment. Here are the four
possible states:
(a) Unexplored area (0): Represents cells that have not yet
been explored.
(b) Free area (1): Represents explored free area.
(c) Obstacle (2): Indicates that a cell is obstructed or contains
an obstacle.
(d) Robot (3): Represents current position of the robot.
Refer to Fig. 2 for a visual representation of these states
with the testing environment we used. The matrix captures
this information, allowing the robots to navigate and make
decisions based on the environment’s characteristics.

<div class="figure container">
  <img class="mx-auto d-block" src="./images/fig2.png" alt="Fig. Robot testing environment and grid map states" width="600" />
  <p class="caption text-center">Fig. Robot testing environment and grid map states</p>
</div>

Each robot is equipped with a proximity sensor, enabling
it to sense and interact with its surrounding environment.
Furthermore, these robots possess the capability to synchronize
the contents of their local memory maps with other robots.
After each time step, subsequent to the update of cell information in its local memory, the robot broadcasts both its current
position and the local map.
In the initial phase of our project, we conducted tests
using two robots. We operated under the assumption that
we had precise knowledge of their starting positions and the
directions they were facing. This allowed us to successfully
manage intercommunication between the robots and integrate
the maps generated by each robot. This led to the creation
of a complete map within each robot. Fig. 3 illustrates the
testing environment we utilized, along with the map that was
generated within a few minutes of operation.

<div class="figure container">
  <img class="mx-auto d-block" src="./images/fig3.png" alt="Fig. Testing environment and generated map for two robots" width="600" />
  <p class="caption text-center">Fig. Testing environment and generated map for two robots</p>
</div>

<div class="figure container">
  <img class="mx-auto d-block" src="./images/Demo.gif" alt="Fig. Testing environment and generated map for two robots" width="600" />
  <p class="caption text-center">Demonstration</p>
</div>

We evaluated the performance of our mapping algorithms
in terms of accuracy, completeness, and scalability.
We also took an approach to mapping with unknown initial
positions and heading directions.

## Conclusion
Swarm robotics can be considered as a core research topic in the scope of multi-agent systems in engineering that leverages the principles of self-organization, decentralized control, and collective intelligence to create adaptive and robust robotic systems capable of performing a wide range of tasks in complex and dynamic environments.

In SLAM-based multi-robot systems, research gaps include complex sensor usage, centralized mapping focus, limited realworld testing, scalability issues with increasing robot numbers, lack of robustness to dynamic environments, potential for sensor fusion techniques, exploration of decentralized communication protocols, and the need for standardized benchmark datasets. These gaps offer opportunities for exploring innovative solutions, advancing system capabilities, and fostering collaboration across disciplines.

Our proposed solutions to these research gaps include utilizing cost-effective robots equipped with simple sensors to streamline operations. Integrating data from various sensors enhances mapping and localization accuracy, optimizing system performance. Implementing decentralized systems with distributed mapping algorithms fosters collaboration among robots, improving overall mapping efficiency. These innovative approaches aim to address existing challenges and propel advancements in multi-robot system capabilities.

However, SLAM with swarm robotics is still a relatively new concept, lacking well-defined frameworks and results. It is believed that Swarm SLAM can be particularly advantageous in producing abstract maps and operating under time or cost constraints. Future directions and potential advancements in Swarm SLAM involve the development of advanced cooperative localization methods, integration of machine learning techniques for improved mapping and localization.

 
## Publications
<!--[//]: # "Note: Uncomment each once you uploaded the files to the repository" -->

1. [Semester 7 report](./Semester_7_report.pdf)
2. [Semester 7 slides](./Semester_7_slides.pdf)
<!-- 3. [Semester 8 report](./) -->
<!-- 4. [Semester 8 slides](./) -->
<!-- 5. Author 1, Author 2 and Author 3 "Research paper title" (2021). [PDF](./). -->


## Links

[//]: # ( NOTE: EDIT THIS LINKS WITH YOUR REPO DETAILS )

- [Project Repository](https://github.com/cepdnaclk/e18-4yp-PeraSwarm-Simultaneous-Localization-and-Mapping-in-Mixed-Reality-Environment)
- [Project Page](https://cepdnaclk.github.io/e18-4yp-PeraSwarm-Simultaneous-Localization-and-Mapping-in-Mixed-Reality-Environment/)
- [Pera Swarm - GitHub Organization](https://github.com/pera-swarm)
- [Pera Swarm - Website](https://pera-swarm.ce.pdn.ac.lk/)
- [Department of Computer Engineering](http://www.ce.pdn.ac.lk/)
- [University of Peradeniya](https://eng.pdn.ac.lk/)

[//]: # "Please refer this to learn more about Markdown syntax"
[//]: # "https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet"
