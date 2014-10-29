import urllib2
import os
import threading
from math import *

TILEHREF_PREFIX="http://readymap.org/readymap/tiles/1.0.0/7/"

def tile2Key(tile_x,tile_y,level):
	quadkey=""
	for l in range(level):
		m=(tile_x>>l)&1
		n=(tile_y>>l)&1
		quadkey+=chr((ord('0')+(m<<1)+n))
	m=(tile_x>>level)&1
	n=1

	quadkey+=chr((ord('0')+(m<<1)+n))
	return quadkey
def tile2Key_2(tile_x,tile_y,level):
	quadkey=""
	for l in range(level):
		m=(tile_x>>l)&1
		n=(tile_y>>l)&1
		quadkey+=chr((ord('0')+(m<<1)+n))
	m=(tile_x>>level)&1
	n=0

	quadkey+=chr((ord('0')+(m<<1)+n))
	return quadkey
def tile2Key_3(tile_x,tile_y,level):
	quadkey=""
	m=(tile_x>>0)&1
	n=0
	quadkey+=chr((ord('0')+(m<<1)+n))
	for l in range(level):
		m=(tile_x>>(l+1))&1
		n=(tile_y>>l)&1
		quadkey+=chr((ord('0')+(m<<1)+n))
	return quadkey
def tile2Key_4(tile_x,tile_y,level):
	quadkey=""
	m=(tile_x>>level)&1
	n=0
	quadkey+=chr((ord('0')+(m<<1)+n))
	for l in range(level):
		m=(tile_x>>(l))&1
		n=(tile_y>>l)&1
		quadkey+=chr((ord('0')+(m<<1)+n))
	return quadkey
def tms_tile2Key(tile_x,tile_y,level):
	return
def test():
	level=2
	n=(1<<level)
	m=n<<1
	keyDict={}
	keyDict_bro={}
	for i in range(m):
		for j in range(n):
			key=tile2Key(i,j,level)
			print i,j,key
			if keyDict.has_key(key):
				print "Key Collision!"

				keyDict[key]=-1
			else:
				keyDict[key]=(i,j)

			# key=tile2Key_4(i,j,level)
			# print i,j,key
			# if keyDict_bro.has_key(key):
			# 	print "Key Collision!"

			# 	keyDict_bro[key]=-1
			# else:
			# 	keyDict_bro[key]=(i,j)
	print keyDict.keys()
	print keyDict.values()

	# print keyDict_bro.keys()
	# print keyDict_bro.values()

def retrieveTile(tileHref,levelCount):

	for level in range(8,levelCount):
		path="./level_"+str(level)
		if not os.path.exists(path):
			# os.remove(path)
			os.mkdir(path)

		level_sz=1<<level
		for tile_x in range(0,level_sz):
			for tile_y in range(level_sz):
				imgURL=tileHref+str(level)+"/"+str(tile_x)+"/"+str(tile_y)+".jpg"
				data=urllib2.urlopen(imgURL).read()
				if data:
					quadKey=tile2Key(tile_x,tile_y,level)
					fout=open(path+"/"+quadKey+".jpg","wb")
					fout.write(data)
					fout.close()
					print imgURL," Downloaded."

				else:
					print imgURL," :Invalid Image."


	return

#rect: [tlx,tly,brx,bry]
def PRINTEXCEPTION(tx,ty,level,errorType,url,e):
	global fout
	global mu
	if mu.acquire():
		print errorType,"Error,\t ErrorCode:"
		print url
		mu.release()
	return
class downloader(threading.Thread):
	def __init__(self,level,rect):
		super(downloader, self).__init__()
		self.__level=level
		self.__numCols=1<<self.__level
		self.__numRows=1<<self.__level
		self.__rect=rect
		(self.__tlx,self.__tly,self.__brx,self.__bry)=rect
		self.__path="./level_"+str(level)
		if not os.path.exists(self.__path):
			os.mkdir(self.__path)
		
	def run(self):
		global mu
		for tile_x in range(self.__tlx,self.__brx):
			for tile_y in range(self.__tly,self.__bry):

				imgURL=TILEHREF_PREFIX+str(self.__level)+"/"+str(tile_x)+"/"+str(tile_y)+".jpg"
				try:
					response=urllib2.urlopen(imgURL)
					data=response.read()
					# if mu.acquire():
					# 	if data:
					# 		quadKey=tile2Key_2(tile_x,tile_y,level)
					# 		fout=open(self.__path+"/"+quadKey+".jpg","wb")
					# 		fout.write(data)
					# 		fout.close()
					# 		# print imgURL," Downloaded."
					# 	mu.release()
					quadKey=tile2Key_2(tile_x,tile_y,self.__level)
					fout=open(self.__path+"/"+quadKey+".jpg","wb")
					fout.write(data)
					fout.close()
					# print imgURL," Downloaded."
				except urllib2.HTTPError,e:

					PRINTEXCEPTION(tile_x,tile_y,self.__level,"HTTPError",imgURL,e)
				except IOError,e:
					PRINTEXCEPTION(tile_x,tile_y,self.__level,"IOError",imgURL,e)
		return
		




if __name__=="__main__":
	print "Download Tile Image from Webserver: ",TILEHREF_PREFIX
	if os.chdir("E:/TMSTiles_CompleteData"):
		print "Success"
	# retrieveTile(TILEHREF_PREFIX,14)
	TASK_BLOCK=64
	Thread=[]
	mu=threading.Lock()
	for level in range(1,14):
		num_cols=1<<level
		TASK_BLOCK=1<<(level-1)
		if num_cols<TASK_BLOCK:
			Thread.append(downloader(level,(0,0,num_cols*2,num_cols)))
		else:
			num_task=num_cols/TASK_BLOCK
			for i in range(num_task*2):
				for j in range(num_task):
					Thread.append(downloader(level,(i*TASK_BLOCK,j*TASK_BLOCK,(i+1)*TASK_BLOCK,(j+1)*TASK_BLOCK)))
	for t in Thread:
		t.start()


