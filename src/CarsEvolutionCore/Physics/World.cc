#include "World.h"

#include <cpputils/logger.hpp>

#include "CarsPopulationData.h"
#include "SimulationData.h"

namespace logger = cpputils::log;  //not used so far




//constructor
cer::physics::World::World(const cer::CarsPopulationData &population,
                           cer::SimulationData *simulation_data)
    : population_(population), simulation_data_(simulation_data) {

    //the scene is initiated
    b2Vec2 gravity;
    gravity.Set(settings.gravity_x, settings.gravity_y);
    m_world = std::make_unique<b2World>(gravity);

    //b2BodyDef bodyDef;
    //m_groundBody = m_world->CreateBody(&bodyDef);

            b2Body* ground = nullptr;
            {
                    b2BodyDef bd;
                    ground = m_world->CreateBody(&bd);

                    b2EdgeShape shape;

                    b2FixtureDef fd;
                    fd.shape = &shape;
                    fd.density = settings.ground_density;
                    fd.friction = settings.ground_friction;
                    fd.filter.categoryBits = 0x0001;
                    fd.filter.maskBits = 0x0002;

                    const int size=20;  //settings.number_of_stages;
                    const float dx = settings.stage_width_x;

                    //start is at flat plate, size 4*dx, on the ground level
                    shape.Set(b2Vec2(-2*dx, 0.0f), b2Vec2(2*dx, 0.0f));
                    ground->CreateFixture(&fd);

                    float x = 2*dx;
                    float y1 = 0.0f;


                    //można potem zamienić na wektor, jak zdecydujemy
                    //gdzie to przechowujemy

                    float hs[size] = {1.0f, 3.0f, 4.0f, -1.0f, -2.0f, 1.0f,
                                      2.0f, 5.0f, -1.25f, 0.0f,1.0f, 1.0f,
                                      4.0f, 0.0f, 0.0f, -1.0f, -2.0f, -5.0f,
                                      -5.25f, 0.0f};


                     // float hs[size];


                    for (unsigned int i = 0; i < size; ++i)
                    {
                            //h[i]=RandomFloat(-5.0f,5.0f)
                            float y2 = hs[i];
                            shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
                            ground->CreateFixture(&fd);
                            y1 = y2;
                            x += dx;
                    }


                    //wall at the end
                    //remove if track could be extended
                    x += dx;
                    shape.Set(b2Vec2(x, 0.0f), b2Vec2(x, 2*dx));
                    ground->CreateFixture(&fd);

            }
}



std::vector<cer::physics::Car> cer::physics::World::generateCars(){

    //number of cars in simulation
    static const size_t car_No=1; //population_.cars().carsNum();
    static const int BODY_POINTS_NUM = (population_.cars().parametersNum()-2)/2;
    std::cout<<"Body points:"<<BODY_POINTS_NUM<<std::endl;
    //vector of cars
    std::vector<Car> cars;
    auto parametersMatrix = population_.cars();

    std::vector<double>::iterator it;

    //for each car
    for(size_t i=0;i<car_No;i++){

        Car car_t; //temporary object

        car_t.car_num=i;

        //radiuses of wheels are saved
        it=parametersMatrix.begin(i);
        car_t.circle_front.m_radius = *it;
        it++;
        car_t.circle_rear.m_radius = *it;
        it++;


        //body point coordinates
        double x_;
        double y_;

        //coordinates of wheels are set later
        double wheel1_x=0;
        double wheel1_y=0;

        double wheel2_x=0;
        double wheel2_y=0;


        //shape of a car

        int j;
        for(it=parametersMatrix.begin(i)+2,j=0;it!=parametersMatrix.end(i)-1 && j<BODY_POINTS_NUM;++it,++j){

            x_=*it;
            ++it;
            y_=*it;
            logger::info()<< x_ <<" "<< y_;

            car_t.vertices[j].Set(x_,y_);


            /*there is no method for getting coordinates from vertices
            so it is better to save them*/
            if(j==settings.wheel1_point_no){
                wheel1_x=x_;
                wheel1_y=y_;
            }

            if(j==settings.wheel2_point_no){
                wheel2_x=x_;
                wheel2_y=y_;
            }

        }

        car_t.chassis.Set(car_t.vertices, BODY_POINTS_NUM);


        //jezeli koło nie bedzie sie krecic to ten blok skopiowac
        //do wheel 2

        car_t.bd.type = b2_dynamicBody;
        car_t.bd.position.Set(0.0f, 1.0f);
        m_car = m_world->CreateBody(&car_t.bd);

        b2Vec2 axis(0.0f, 1.0f);


        //wheel 1
        m_car->CreateFixture(&car_t.chassis, 1.0f);
        car_t.fdc.shape = &car_t.circle_front;
        car_t.fdc.density = settings.wheel1_density;
        car_t.fdc.friction = settings.wheel1_friction;

        car_t.bd.position.Set(wheel1_x, wheel1_y);  //wheel position
        m_wheel1 = m_world->CreateBody(&car_t.bd);
        m_wheel1->CreateFixture(&car_t.fdc);

        car_t.jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
        car_t.jd.motorSpeed = settings.motor1_speed;
        car_t.jd.maxMotorTorque = settings.motor1_maxTorque;
        car_t.jd.enableMotor = settings.motor1_enable;
        car_t.jd.frequencyHz = settings.motor1_frequencyHz;
        car_t.jd.dampingRatio = settings.wheel1_dampingRatio;
        m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&car_t.jd);



        //wheel 2
        m_car->CreateFixture(&car_t.chassis, 1.0f);
        car_t.fdc.shape = &car_t.circle_rear;
        car_t.fdc.density = settings.wheel2_density;
        car_t.fdc.friction = settings.wheel2_friction;

        car_t.bd.position.Set(wheel2_x, wheel2_y);    //wheel position
        m_wheel2 = m_world->CreateBody(&car_t.bd);
        m_wheel2->CreateFixture(&car_t.fdc);

        car_t.jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
        car_t.jd.motorSpeed = settings.motor2_speed;
        car_t.jd.maxMotorTorque = settings.motor2_maxTorque;
        car_t.jd.enableMotor = settings.motor2_enable;
        car_t.jd.frequencyHz = settings.motor2_frequencyHz;
        car_t.jd.dampingRatio = settings.wheel2_dampingRatio;
        m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&car_t.jd);


        car_t.iter_stopped=0;   //simualtion management default parameter
        car_t.stopped=0;        //simualtion management default parameter

        car_t.fdc.filter.categoryBits = 0x0002;
        car_t.fdc.filter.maskBits = 0x0001;


        printf("%d \n",car_t.car_num);


        cars.push_back(car_t);
    }

    return cars;

}



bool cer::physics::World::runSimulation() {

    auto cars_num = population_.cars().carsNum();
    simulation_data_->reset(cars_num);
    /* for dummy simulation
    auto cars_num = population_.cars().carsNum();
    simulation_data_->reset(cars_num);
    */

    /*
    // dummy simulation
    std::vector<float> speeds(cars_num);
    for (auto &s : speeds) {
      s = float(rand() % 10);
    }
    */


#if defined(_WIN32)
// Enable memory-leak reports
_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif


  float timeStep = settings.m_hertz > 0.0f ? 1.0f / settings.m_hertz : float(0.0f);

  //m_world->SetAllowSleeping(settings.m_enableSleep);
  m_world->SetWarmStarting(settings.m_enableWarmStarting);
  m_world->SetContinuousPhysics(settings.m_enableContinuous);
  m_world->SetSubStepping(settings.m_enableSubStepping);

  //creating physical car objects
  //'population_' was assigned in constructor
  std::vector<Car> cars =generateCars();
  bool stop=0; //flag for simulation control
  int iter=0;

  std::vector<Car>::iterator it;

  b2Vec2 position;
  b2Vec2 last_position;
  b2Vec2 diff_position;   //for b2vec2 only -= operator is defined
  last_position.SetZero();
  float angle;
  //int cars_not_moving;


  while(!stop && iter <settings.sim_max_iter) {
    // simulate computations
    //    logger::info() << "runSimulation" << j;


      /*for each car
       *jeżeli tak zdecydujemy, to można nie liczyć dla samochodów
       *które mają status zatrzymanych*/


      for (it=cars.begin(); it !=cars.begin()+1 /*cars.end()*/ ; ++it) {


        m_world->Step(timeStep, settings.m_velocityIterations, settings.m_positionIterations);

        position = it->bd.position;
        angle = it->bd.angle;
        Position position_={position.x,position.y,angle};
        printf("%d\n",iter/10);

        printf("%d %4.2f %4.2f %4.2f\n",it->car_num, position_.x,position_.y,position_.theta);
        simulation_data_->pushPosition(it->car_num, position_);



        /* checks if car is moving in our definition and
         * if can be counted as stopped.
         * Definition: if length of position shift vector for the car
         * is smaller than 'settings.minimumLength_of_vector',
         * the car is not moving. If it is not moving in
         * 'settings.max_car_iter' iterations, it is counted as stopped.
         *
         */
        diff_position=position;
        diff_position-=last_position;   //for b2vec2 only -= operator is defined

        if(diff_position.Length()<settings.minimumLength_of_vector)
            (it->iter_stopped)++;
        else
            it->iter_stopped=0;

        position=last_position;

        if(it->iter_stopped>settings.max_car_iter)  //flaga up
            it->stopped=1;

        iter++;


    }// for each car



    //Checks if any car is moving.
    //If not, 'stop' will be left with value 1 and simualtion stopped.

    stop=1;
    for(it=cars.begin();it !=cars.end();it++)
        if(!it->stopped){
            stop=0;
            break;
        }


  }//simulation loop 'while'


  return true;
}

