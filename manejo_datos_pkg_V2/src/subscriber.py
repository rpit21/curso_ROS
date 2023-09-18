#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import LaserScan 
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32
import numpy as np



def callback(data:LaserScan):

    distancias=np.array([])
    angulos=np.array([])

    rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.header)
    lens= int((data.angle_max - data.angle_min) / data.angle_increment)

    for i in range(0,lens):

        anguloActual=(data.angle_min + i * data.angle_increment)#*180/3.14159
        rangoActual=data.ranges[i]
        intensidad=data.intensities[i]

        rospy.loginfo("Angulo: %f, Rango: %f, Intensidad:%f", anguloActual, rangoActual, intensidad)

        # Escojo aquellos valores y angulos en un array que esten a una distancia cercana (Modificar para que se amplie su rango de seleccion)
        if rangoActual<=7.0 and (anguloActual<=1.57 or anguloActual>=4.71):
            distancias=np.append(distancias,rangoActual)
            angulos=np.append(angulos,anguloActual)

    #llamo a la funcion que detecta obstaculos
    obstaculos=detec_obstacle(distancias,angulos)

    #llamo a la funcion que modifica la velocidad lineal y angular 

    v_angular,v_lineal=calcular_velocidades(obstaculos)


    pub.publish(v_lineal)


def calcular_velocidades(obstaculos):
    if len(obstaculos)==0:
        V_angular=0
        V_lineal=50
    else:
        #Calculo de la fuerza repulsiva basada en las coordenadas de los obstaculos
        k_repulsivo=1
        force_repulsiva_x=0.0
        force_repulsiva_y=0.0

        for obstacle_x, obstacle_y in obstaculos:
            gradient_x=-obstacle_x
            gradient_y=-obstacle_y
            force_repulsiva_x=k_repulsivo*gradient_x
            force_repulsiva_y=2.3*gradient_y
        
        # Calculo de la velocidad angular 
        V_angular=force_repulsiva_y

        #calculo de la velocidad lineal
        V_lineal=50+force_repulsiva_x

    return V_angular, V_lineal

def detec_obstacle(distancias, angulos):
    #convierto las distancias y angulos en coordenadas cartesianas
    x=distancias*np.cos(angulos)
    y=distancias*np.sin(angulos)

    #Aplico filtro para identificar puntos cercanos entre si (modificar para mayor filtro)
    min_cluster_dis=0.5
    clusters=[]
    current_cluster=[]

    for i in range(len(x)):
        if i==0 or np.sqrt((x[i]-x[i-1])**2 +(y[i]-y[i-1])**2)<=min_cluster_dis:  #identifico la cercania de los puntos entre si y evaluo
            current_cluster.append((x[i],y[i]))
        else:
            clusters.append(current_cluster)
            current_cluster=[(x[i],y[i])]

    
    #identificar obstaculos basados en le tamanio del Cluster (la cantidad de puntos existentes) (Modificar para mayor filtro)
    min_cluster_size=10
    obstacles=[]

    for cluster in clusters:
        if len(cluster)>=min_cluster_size:
            x_values, y_values= zip(*cluster)
            x_obstacle=np.mean(x_values)
            y_obstacle=np.mean(y_values)
            obstacles.append((x_obstacle,y_obstacle))

    return obstacles

if __name__ == '__main__':
    try:
        #Inicializamos nuestro nodo
        rospy.init_node('LectorLidar', anonymous=False)

        #funcion para publicar
        pub = rospy.Publisher('Evasion', Float32, queue_size=10)

        #funcion para susbcribirse
        rospy.Subscriber("scan", LaserScan, callback)

        rospy.spin()

    except rospy.ROSInterruptException:
        pass


    

    

    

