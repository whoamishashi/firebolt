
# coding: utf-8

# <div style="text-align: right"> Jordi Oller Sànchez </div>
# 
# # Star tracker
# 
# ## 1. Introduction and method used
# 
# This star tracker resolves the lost in space problem using the angle detection method
# <img src="triangle_angles.png" alt="Drawing" style="width: 300px;"/>
# In our algorithm we choose as the central star Sc and we calculate alpha and gamma, the beta angle is not calculated since by the propierties of the triangle if we find a triangle that has the same alpha and gamma as the one we are looking for, beta will not differ from the calculated beta. In order to increase the accuracy and the changes of getting false positions or multiple stars that are coherent with our angles, the triangle method is done several times for our central star. This example is configured for making 7 triangles with the neighbour stars. The first triangle is made with the nearest star and the 2n nearest one, the second triangle is made with the 2nd and the 3rd and so on.
# 
# For detecting stars in the image obtained from the PiCamera we use the library openCV, specialised on detecting patterns on images.
# 
# Another important thing to mention is that the Star Catalog, was generated from scratch with the image SMDMiniStarsFinal that was provided in the documentation also using OpenCV and exactly the same method that is used for detecting stars on our images from the camera. This action pursues two main objectives, the first one is to adapt the database to only storage the data needed for detecting stars with our method. The second main objective is improve accuracy: In some cases, some stars are or overlaped or so close each other than the image detection of stars in the image only detects one star so, it makes sense to create a new catalog with the same algorithm so those singularities can be resolved.
# 
# ## 2. General structure of the program
# 
# 
# ## 3. Problems encountered
# 
# After the initial developement of our Star Tracker, two main problems where found:
# 
# ### 3.1. Almost equal distance stars:
# In the case 2 stars have almost the same distance to our target star, there is a chance that, given the resolution of the PiCamera and the slight errors in position when detecting the positions of the stars inside our Pi photo, the second nearest star to our target star according to our Star Catalog, might be seen as the third nearest star, and the third one be seen as the second. We must remember that the first triangle for that particular star in our database was calculated with the first and second star nearest to our target star, not the second and third, since angles beta and gamma will be wrong, that triangle will not be detected and, by applying the same logic, the second triangle will also fail. This effect can not be avoided, but can be mitigated creating alternative triangles that switch the order of the stars in the cases where they have aproximately the same distance with the central star. If some triangles are not detected, this alternative triangles are used to refine the solution in a 2nd iteration
# 
# 
# We also have to take into account that this effect is inversely proportional to the height of the camera in the floatsat vehicle.
# 
# ### 3.2. Neighbour stars not in the field of view:
# 
# There can be some cases that for having a low density of stars in the neighbourhood of our central star or by having a central star near one of the boundaries of the photo because not having stars in the center of the image, that the near stars from the central star are not choosed correctly because some of them are outside the field of view, this problem will lead to a wrong calculation of the triangles and will lead to don't detect the actual position of the Floatsat vehicle.
# 
# In order to avoid that, an algorith check that all the stars we need for making our calculus are situated inside a circle contained in the image. If that is not the case, another central star is choosen.
# 
# 
# ## 4. Star tracker code demostration

# Importing needed libraries

# In[31]:


import numpy as np
import cv2
import math
import scipy as sc
import scipy.spatial as sp
from matplotlib import pyplot as plt
import multiprocessing
from multiprocessing import Pipe
import time


# Given a set of stars, this function returns the central star of the image plus the set of the stars without the central star

# In[32]:


def findcenter (data, x, y, index):
    d = []
    i = 0
    while i < len(data):
        d.append(((data[i][0]-x/2)**2 + (data[i][1]-y/2)**2)**0.5)
        i = i+1
    data = np.c_[data, d]
    data = data[np.argsort(data[:,2])]
    data = np.delete(data, 2, 1)
    center = np.array(data[index])
    data = np.delete(data, index, 0)
    return center, data


# Function for finding the distance of every star to the central star 

# In[33]:


def finddistance (data, center):
    i = 0
    d = np.zeros(len(data))
    while i < len(data):
        d[i] = sp.distance.euclidean(data[i], center)
        i=i+1
    data = np.c_[data, d]
    data = data[np.argsort(data[:,2])]
    return data


# The following function finds beta and gamma for the triangles that are made between the target star and the neighbour stars. The the angle beta is equal to angle alpha in the image shown at the start of the document.

# In[34]:


def findangles(data, center, n):
    beta = np.zeros (n)
    gamma = np.zeros (n)
    i=0
    while i<n:# compute area of every triangle and the polar moment
        a = sp.distance.euclidean(data[i], center)
        b = sp.distance.euclidean (data[i+1], center)
        c = sp.distance.euclidean (data[i], data[i+1])
        beta[i] = np.arccos((b**2-a**2-c**2)/(-2*a*c))
        gamma[i] = np.arccos((c**2-a**2-b**2)/(-2*a*b))
        i = i+1
    result = np.c_[beta, gamma]   
    return result


# This functions finds beta and gamma from the alternative triangles:

# In[35]:



def alternativetriangles (data, errD):
    datalist = []
    i = 0
    while i+1 < len(data):
        data2 = data.copy()
        dif =abs(data2[i, 2] - data2[i+1, 2])
        if i == 0:
            data0 = data.copy()
            data0 = np.delete(data0, 2, 1)
            datalist.append(data0)
        if dif < errD:
            temp = data[i]
            data2[i] = data[i+1]
            data2[i+1] = temp
            data2 = np.delete(data2, 2, 1)
            datalist.append(data2)
        i = i+1  
    return datalist


# In this function we calculate the angle between the line that unites the target star and the neighbour star and
# the x coordinate of the camera image frame

# In[36]:


def findorientation (center, d, n):
    i = 0
    angles = np.zeros(n+1)
    while i < n+1:
        a = sp.distance.euclidean(d[i], center)
        point = [d[i,0], center[1]]
        c = sp.distance.euclidean(point, center)
        angle = np.arccos(c/a)
        if d[i,0] > center[0] and d[i,1] < center[1]:
            angles[i] = angle
        if d[i,0] < center[0] and d[i,1] < center[1]:
            angles[i] = math.pi - angle
        if d[i,0] < center[0] and d[i,1] > center[1]:
            angles[i] = math.pi + angle
        if d[i,0] > center[0] and d[i,1] > center[1]:
            angles[i] = 2*math.pi - angle
        i +=1
    return angles


# This function pretends to solve the problem of our algorithm choose a star in a region with low density of stars or near the boundaries of the photos. Returns the number of stars inside the biggest circle that can be fitted in our photo, with center our candidate of target star

# In[37]:


def starsincircle (center, data):
    numbers = np.c_[center[0], width-center[0], center[1], height - center[1]]
    radius = np.amin(numbers)
    #print(radius)
    result = np.where(data[:,[2]]<radius)
    return len(result[0])
    


# This function compares all the angles obtained for our neighbour stars and compares it with the ones found in our database. A list with the id of the stars that can potentially be our target star is attached (items list).Every beta and gamma of every triangle of all the stars to be analized is compared, if beta and gamma of the triangle is inside the margin error defined by the variable err, a 1 is written in a list called boolean , so this gives 1 point to that particular star to be our target star. Additionaly,the position of the triangle in the database is written in the list asoc

# In[38]:


def comparison (db, err, result, items, boolean, asoc):
    dim =db.shape
    i = 0
    while i < len(items):
        temp = db[items[i]]
        #boolean, asoc = comparison (result, temp, err, boolean, asoc)
        j = 0
        while j< len (result):
            data = result[j]
            A_dif = temp[:,0]
            A_dif = np.absolute(A_dif - data[0])
            index = np.argmin(A_dif)
            #new mehod -> more computing time but less false negatives#
            flag_beta = np.isclose(temp[index,0], data[0], atol = err[j,0])
            flag_gamma = np.isclose(temp[index,1], data[1], atol = err[j,1])
            if flag_beta == True & flag_gamma == True:
                boolean[items[i], index, 0]= 1
                asoc[items[i], j, 0] = index          
            j +=1
        i +=1 
    return boolean, asoc


# Same function as before but made compact form for optimizing time. While using this function only in lost in space problem and enabling multicore, the speed of the computation time is greatly improved

# In[39]:


def comparison_multicore (db, err, result, boolean, asoc, bool_p, asoc_p):
    dim =db.shape
    i = 0
    for temp in db:
        j = 0
        for j in range(len (result)):
            index = np.argmin(np.absolute(temp[:,0] - result[j,0]))
            flag_beta = np.isclose(temp[index,0], result[j,0], atol = err[j,0])
            if flag_beta == True:
                flag_gamma = np.isclose(temp[index,1], result[j,1], atol = err[j,1])
                if flag_gamma == True:
                    boolean[i, index, 0]= 1
                    asoc[i, j, 0] = index          
        i +=1
    bool_p.send(boolean)
    bool_p.close()
    asoc_p.send(asoc)
    asoc_p.close()


# For every star, a sum of all points obtained in the comparison is done

# In[40]:


def pointrating (boolean, dim, points):
    i = 0
    while i < dim[0]:
        temp = boolean[i]
        points[i] = np.sum(temp)
        i +=1
    return points


# After our initial estimation of identifying our target star, if there are more than one possible stars or the detection of one of the two angles contained in the triangle fails, this function is initialized in order to check if that failure was a consequence of the near stars problem:

# In[41]:


def refinesolution (tupledata, boolean, asoc):
    j = 0
    while j < len(datalist):
        result = findangles (datalist[j], center, n)
        #print (result)
        i = 0
        while i < len(tupledata[0]):
            items = tupledata[0]
            boolean, asoc = comparison (db, err, result, items, boolean, asoc)
            i+=1
        j+=1
    return boolean, asoc


# Main program:

# In[42]:



def mainprogram (pts, p):
    global n
    global datalist
    global db
    global center
    global err
    global errD
    global boolean_c1
    global boolean_c2
    global boolean_c3
    global asoc_c1
    global asoc_c2
    global asoc_c3
    
    angl_camera_satel = 90
    angl_camera_cm = 31.72
    offset = np.matrix([[0.08731],[0.05397]]) # offset [x, y] of our camera respect the center of our satelite
    n = 7 # number of triangles made
    n_cut = 5 # minimum triangles to be detected for considering our solution valid
    errA = 0.05 # maximum relative error for every angle in the triangle
    errD = 10 # pixels <-Depends a lot in the height of the camera
    scale = 0.31 # scale in pixels between the image obtained from the camera and the starcatalog photo
    val = False
    # In this loop our intention is to secure that, when we choose a target star of our image for calculating the
    # satellite's position, the necessary stars for making our calculus are all contained in an imaginary cirlce 
    # inside our photo. Doing that we ensure that there are no stars nearer our target star that the ones thare
    # are contained inside our photo.
    while val == False:
        center, pts2 = findcenter(pts, width, height, p)
        pts2 = finddistance (pts2, center)
        number = starsincircle(center, pts2)
        if number >= n:
            val == True
            break
        else:
            p+=1
        if p+1 == len(pts):
            n = n-1
        if n==0:
            print('ERROR')
            break
    print('n compatible with this image:', n)
    print ('center', center)
    # Now we calculate the extra triangles
    datalist = alternativetriangles (pts2, errD)
    result = findangles (datalist[0], center, n)
    # We load our database and we split it in 3 subsets (1 for every core of Raspberry Pi), also we create 3 lists
    # to record the points obtained for every star (boolean_c1, boolean_c2, boolean_c3) and we create another list
    # for knowing which triangles from the database are equivalent to which ones from our image 
    db = np.load ('position.npy')
    dim = db.shape
    db_c1 = db[0:972]
    db_c2 = db[972:1945]
    db_c3 = db[1945:2917]
    boolean_c1 = np.zeros((972, dim[1], 1))
    asoc_c1 = np.full((972, dim[1], 1), -1)
    boolean_c2 = np.zeros((973, dim[1], 1))
    asoc_c2 = np.full((973, dim[1], 1), -1)
    boolean_c3 = np.zeros((972, dim[1], 1))
    asoc_c3 = np.full((972, dim[1], 1), -1)
    # We define pipes in order to extract the results from every core
    parent_bool_p1, child_bool_p1 = Pipe()
    parent_bool_p2, child_bool_p2 = Pipe()
    parent_bool_p3, child_bool_p3 = Pipe()
    parent_asoc_p1, child_asoc_p1 = Pipe()
    parent_asoc_p2, child_asoc_p2 = Pipe()
    parent_asoc_p3, child_asoc_p3 = Pipe()
    err = np.absolute(result*errA)
    # We define and launch the beta and gamma angle comparison function in the 3 cores of Raspberry
    p1 = multiprocessing.Process(target = comparison_multicore,args = (db_c1, err, result, boolean_c1, asoc_c1, child_bool_p1, child_asoc_p1))
    p2 = multiprocessing.Process(target = comparison_multicore,args = (db_c2, err, result, boolean_c2, asoc_c2, child_bool_p2, child_asoc_p2))
    p3 = multiprocessing.Process(target = comparison_multicore,args = (db_c3, err, result, boolean_c3, asoc_c3, child_bool_p3, child_asoc_p3))
    p1.start()
    p2.start()
    p3.start()
    p1.join()
    p2.join()
    p3.join()
    
    # after obtaining the results for every core, we join them
    boolean = np.concatenate((parent_bool_p1.recv(), parent_bool_p2.recv(), parent_bool_p3.recv()), axis = 0)
    asoc = np.concatenate((parent_asoc_p1.recv(), parent_asoc_p2.recv(), parent_asoc_p3.recv()), axis = 0)
    # We obtain the points for every star
    points = np.zeros((dim[0]))
    points = pointrating(boolean, dim, points)
    star = np.argmax(points)
    maxpoints = points[star]
    # after the best star is found, the id is obtained from the file points.npy
    pts = np.load('points.npy')
    print('1st iteration')
    print('------VALUE------')
    located_star = pts[star]
    print (located_star)
    print('-----------------')
    print(asoc[star])
    tuplebelowpoints = np.where(points==maxpoints-1)
    tuplemaxpoints = np.where(points==maxpoints)
    tuplebelowpoints = np.where(points==maxpoints-1)
    if (len(tuplemaxpoints[0])>1 and len(tuplemaxpoints[0])<4) or (maxpoints!=n and len(tuplemaxpoints[0])>=0 and len(tuplemaxpoints[0])<4):
        boolean, asoc = refinesolution(tuplemaxpoints, boolean, asoc)
        points = pointrating(boolean, dim, points)
        star = np.argmax(points)
        maxpoints = points[star]
        print('2nd iteration')
        print(asoc[star])
        print(maxpoints)
        print (star)
        print('------VALUE------')
        located_star = pts[star]
        print (located_star)
        print('-----------------')
        tuplemaxpoints = np.where(points==maxpoints)
        tuplebelowpoints = np.where(points==maxpoints-1)
    print('maxpoints: ', maxpoints)
    print(n/2+1)
    if len(tuplemaxpoints[0])== 1 and maxpoints > n_cut:
        asocs = asoc[star]
        target = np.where(asocs==-1)
        asocs = np.delete(asocs, target[0])
        print (asocs)
        asocs2 = asocs + 1
        i = 0
        r = []
        val = -1
        while i < len(asocs):
            if val != asocs[i]:
                 r.append(asocs[i])
            r.append(asocs2[i])
            val =asocs2[i]
            i +=1
        angles = findorientation (center, datalist[0], n)
        db_angl = np.load('angle.npy')
        angles_map = db_angl[star]
        i = 0
        dif = []
        if len(r) > len(angles):
            t = len(angles)
        else:
            t = len(r)
        while i+1 < t:
            difference = angles[i] - angles_map[r[i]]
            if difference < 0:
                difference = difference + 2*math.pi
            i +=1
            dif.append(difference)
        orient = np.sum(dif)/len(dif)
        variance = np.var(dif)
        rel_var = variance/orient*100
        # now it is time to discard measurements that differ 
        while rel_var > 5:
            dif_mean = abs(dif - orient)
            worst_angle = np.argmax(dif_mean)
            dif = np.delete (dif, worst_angle, 0)
            orient = np.sum(dif)/len(dif)
            variance = np.var(dif)
            rel_var = variance/orient*100
        orient_camera_360 = orient*180/(math.pi)
        variance_360 = variance*180/(math.pi)
        print('Stars used for angle calculation: ', len(dif))
        p = -1
        diference = [scale*center[0] - width/2, scale*(center[1] - height/2)]
        center_camera_m = (located_star - diference)*(2/5669)
        orient_360 = orient_camera_360 + angl_camera_satel
        if orient_360 >= 360:
            orient_360 = orient_360 - 360
        print ('Angle in degrees:',orient_360)
        print ('Variance in degrees: ', variance_360)
        print ('Camera position (m):', center_camera_m)
        M =np.matrix([[np.cos(np.deg2rad(orient_360)), - np.sin(np.deg2rad(orient_360))],
                      [np.sin(np.deg2rad(orient_360)), np.cos(np.deg2rad(orient_360))]])
        board_frame_offset = M.dot(offset)
        position_satel = center_camera_m + board_frame_offset.T
        print ('Satellite position (m):', position_satel)
    return p


# Here the program stars. First of all we load the image from the camera and we process it in order to filter undesired pixels

# In[51]:


start_time = time.time()
img1 = cv2.imread('image1_2.jpg',0)
plt.imshow(img1)
plt.show()


# Now we start processing the image

# In[ ]:


img1 = cv2.GaussianBlur(img1, (5, 5), 0)
img1 = cv2.threshold(img1, 140, 255, cv2.THRESH_BINARY)[1]


# Now we let OpenCV to do the magic:

# In[44]:


params = cv2.SimpleBlobDetector_Params()
params.blobColor = 255
#params.filterByColor = True
#params.filterByArea = True
#params.minArea = 2
#params.minCircularity = 0.1
detector = cv2.SimpleBlobDetector_create(params)
kp = detector.detect(img1)
img3 = cv2.drawKeypoints(img1, kp, None, color=(255,0,0))
plt.imshow(img3)
#cv2.imwrite('positions_image.png',img3)
plt.figure(figsize=(20,40))
plt.show()
plt.figure(figsize=(20,10))
pts = np.array([p.pt for p in kp])
#print (pts)
height, width = img1.shape


# In this image we can see the result of the gausian Blur and binary threshold of the image and superposed we can observe red circles that are the stars that have been detected

# Loop that runs the mainprogram until an acceptable result is found. For every iteration, the target star is changed:

# In[45]:


p = int()
p = 0
while p != -1:
    print (p)
    p = mainprogram (pts, p)
    if p !=-1:
        p +=1


# In[46]:


print("--- %s seconds ---" % (time.time() - start_time))


# In[47]:


#END OF THE PROGRAM#


# ### Changelog
# #### V1.7:
# - Optimization of comparison function. Increased performance by x2
# #### V1.6:
# - implementation of multiprocessing in comparison function. Cores enabled = 3 
# - Increased performance by x3 (obviously)
# #### V1.5:
# - optimization of some functions for increasing performance. Increased the speed of the calculus by 43%   
# #### V1.4:
# - optimization of the code
# - last resource option: if data of the star choosen for making the calculus are not trustable, another star is choosen 
# #### V1.3:
# - Added algorithm for preventing the selection of a star near image boundaries problem
# - Improvements in orientation angle accuracy
# #### V1.2:
# - Deleted angle alpha from the calculus
# - Added near equal distance star problem algorithm solution
# #### V1.1: 
# - Optimization of the code
# - Added initial angle detection support
# #### V1.0: 
# - Initial release

# In[49]:


##Parameters to adjust for every satellite##
# errD = Ask Jordi.
# scale = ratio in pixels between the distance of two stars in the camera and the distance of those two stars in the star map.
# angl_camera_satel = Angle between the camera and the satellite. Always positive angle (units in degrees).
# angl_camera_cm = Angle between the camera and the center of mass of the satellite in the star board system coordinates (units in degrees).
# offset = distance between the cm and the camera (units in m).

