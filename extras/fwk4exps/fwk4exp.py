import ConfigParser
import subprocess
import multiprocessing
import itertools
from thread import start_new_thread, allocate_lock
from multiprocessing import Process, Value, Lock
import ctypes
import copy_reg
import types
import random
import time
import numpy as np
import math
from random import shuffle
from scipy.stats import t
from scipy import stats
import os
from sets import Set
import sys

lock = Lock()
shared_times = []
shared_boxes = []
shared_gains = []
shared_run = []
shared_avgtime = []
shared_stddev = []
shared_runinst = []
shared_L = []
shared_W = []
shared_stop = 0

class Algo:
	def __init__(self, configParser, name):
		self.name = name
		if configParser.has_option(name, 'output_dir'):
			self.output_dir = configParser.get(name, 'output_dir')
		else:
			self.output_dir = name
		self.ex = configParser.get(name, 'exec')
		self.params = configParser.get(name, 'params')
		self.params = self.params.replace('_ALGO', self.ex);
		self.params = self.params.replace('_MAXTIME', configParser.get('base', 'maxtime'));
		self.outputpos_times = configParser.get(name, 'outputpos_times')
		self.outputpos_boxes = configParser.get(name, 'outputpos_boxes')


	def run(self, inst, seed):
		run_exec = self.ex + " " + self.params.replace('_INSTANCE', inst)
		run_exec = run_exec.replace('_SEED', str(seed))
		print run_exec
		output = subprocess.check_output(run_exec,shell=True,)
		print output
		output = output.splitlines()[-1]

		try:
			return (float(output.split()[int(self.outputpos_times)]), float(output.split()[int(self.outputpos_boxes)]), output)
		except ValueError:
			print "error with instance " + inst.split('/')[-1] + "\n" + output
			return -2.0


class Config:
	def __init__(self, filename):
		configParser = ConfigParser.RawConfigParser()
		configParser.read(filename)
		self.instances = configParser.get('base', 'test_instances')
		self.mintimes = []
		for mintime in configParser.get('base', 'mintime').split():
			self.mintimes.append(float(mintime))

		self.factors = []
		for factor in configParser.get('base', 'factors').split():
			self.factors.append(float(factor))

		self.maxtime = float(configParser.get('base', 'maxtime'))
		self.sig_gain = float(configParser.get('base', 'sig_gain'))
		self.max_seeds = int(configParser.get('base', 'max_seeds'))
		self.min_seeds = int(configParser.get('base', 'min_seeds'))
		f = open(self.instances)
		self.instances = f.read().splitlines()
		f.close()
		self.idx2inst = range(len(self.instances))
		for i in range(len(self.instances)): self.idx2inst[i]=i
		random.seed(0)
		shuffle(self.idx2inst)


		self.algos = []
		for algo_name in configParser.get('base', 'algo_names').split():
			if configParser.has_option(algo_name, 'tuning_param'):
				tuning_param=configParser.get(algo_name, 'tuning_param')
				for value in configParser.get(algo_name, 'tuning_values').split():
					algo=Algo(configParser,algo_name)
					self.algos.append(algo)
					algo.params = algo.params.replace('_%s'%tuning_param, value);
					algo.name = algo.name + "_" + tuning_param + value
			else:
				algo=Algo(configParser,algo_name)
				self.algos.append(algo)

def onKeyPress(event):
	global shared_stop
	shared_stop=1

def read_times(name,i,config):
	global shared_times, shared_boxes
	shared_times[i,:] = -1.0
	shared_boxes[i,:] = -1.0
	try:
		f = open("%(dir)s/%(name)s_times.out" % {"dir":config.algos[i].output_dir,"name":name}, )
		f2 = open("%(dir)s/%(name)s_boxes.out" % {"dir":config.algos[i].output_dir,"name":name}, )
		idx = 0
		for lines in f:
			#~ print lines.split()
			seed=0
			if idx==len(config.instances): break
			for line in lines.split():
				if seed==config.max_seeds: break
				shared_times[i,idx + len(config.instances)*seed ] = float(line)
				if shared_times[i,idx + len(config.instances)*seed]==-1.5: shared_times[i,idx + len(config.instances)*seed]=-1.0
				seed += 1
			idx+=1

		idx = 0
		for lines in f2:
			#~ print lines.split()
			seed=0
			if idx==len(config.instances): break
			for line in lines.split():
				if seed==config.max_seeds: break
				#shared_boxes[i,idx + len(config.instances)*seed ] = float(line)
				if shared_boxes[i,idx + len(config.instances)*seed]==-1.5: shared_boxes[i,idx + len(config.instances)*seed]=-1.0
				seed += 1
			idx+=1

		#~ print shared_times[i]
		f.close()
		f2.close()
	except IOError:
		None

	timesA = []
	for elem in (j for j in shared_times[i,:] if j > 0.0):
		timesA.append(elem)
	#shared_run[i]=len(timesA)

def write_times(name,i,config):
	global shared_times, shared_boxes
	try:
		os.mkdir(config.algos[i].output_dir)
	except OSError:
		None

	f = open("%(dir)s/%(name)s_times.out" % {"dir":config.algos[i].output_dir,"name":name}, "w")
	f2 = open("%(dir)s/%(name)s_meantimes.out" % {"dir":config.algos[i].output_dir,"name":name}, "w")


	for j in  range(len(config.instances)):
		for k in range(config.max_seeds):
			f.write("%.4f " % shared_times[i][k*len(config.instances)+j])
		f.write("\n")

		(mean,h,n) = mean_error(config, i, j, config.max_seeds)
		f2.write("%.4f" % mean)
		f2.write("\t%.2f\n" % h) #desviacion estandar
		#f2.write(" (%d)\n" % n)

	f.close()
	f2.close()

	f = open("%(dir)s/%(name)s_boxes.out" % {"dir":config.algos[i].output_dir,"name":name}, "w")
	f2 = open("%(dir)s/%(name)s_meanboxes.out" % {"dir":config.algos[i].output_dir,"name":name}, "w")

	f.close()
	f2.close()

def final_tables(config):
	f = open("final_results.txt", "w")
	f.write("solved instances in standard strategy times\n")



	for mintime in config.mintimes:
		times=np.zeros(shape=(len(config.algos)))
		nb_inst=np.zeros(shape=(len(config.algos), len(config.factors)))
		nb_inst2=np.zeros(shape=(len(config.algos), len(config.factors)))
		instances=Set()
		for i in range(len(config.instances)):
			if config.instances[i].endswith('*'): continue
			for id_algo in range(len(config.algos)):
				n=0; totaltime=0
				for kk in range(config.max_seeds):
					if shared_times[id_algo,i+kk*len(config.instances)]>=0:
						totaltime+=shared_times[id_algo,i+kk*len(config.instances)]
						n+=1
				times[id_algo]=float(totaltime)/float(n)

			if times.min()<config.maxtime and times.max()>mintime:
				for id_algo in range(len(config.algos)):
					if (times[id_algo]<config.maxtime or times[0]<config.maxtime) and (times[id_algo]>mintime or times[0]>mintime):
						j=0
						for factor in config.factors:
							if times[id_algo]/(times[0]+0.01) < factor:
								nb_inst[id_algo][j]+=1;
							if times[0]/(times[id_algo]+0.01) < factor:
								nb_inst2[id_algo][j]+=1;

							if times[id_algo]/(times[0]+0.01) > config.sig_gain or times[0]/(times[id_algo]+0.01) > config.sig_gain:
								instances.add(i);

							j+=1


		f.write("mintime:%f\n" % mintime)


		for i in range(len(config.factors)):
			if (i==0):
				f.write("<%.2f "% (config.factors[len(config.factors)-i-1]))
			else:
				f.write("[%.2f,%.2f] "% (config.factors[len(config.factors)-i],config.factors[len(config.factors)-i-1]))

		for i in range(len(config.factors)):
			if (i==len(config.factors)-1):
				f.write(">%1.2f "% (1.0/config.factors[i]))
			else:
				f.write("[%1.2f,%1.2f] "% (1.0/config.factors[i],1.0/config.factors[i+1]))

		f.write("\n")

		for id_algo in range(len(config.algos)-1):
			f.write("%10s " % config.algos[id_algo+1].name)
			for i in range(len(config.factors)):
				f.write("%3d "% nb_inst2[id_algo+1][len(config.factors)-i-1])
			for i in range(len(config.factors)):
				f.write("%3d "% nb_inst[id_algo+1][i])
			f.write("\n")

		for i in range(len(config.algos)-1):
			f.write("%s " % config.algos[i+1].name)
		f.write("\n")

		for i in range(len(config.factors)):
			f.write("%f " % (1/config.factors[i]))
			for id_algo in range(len(config.algos)-1):
				f.write("%d/"% nb_inst[id_algo+1][i])
				f.write("%d "% nb_inst2[id_algo+1][i])
			f.write("\n")

		f.write("\n\n")


		f.write("instances with significative gains (%s)\n"%config.sig_gain)
		for j in instances:
			f.write("%s " % config.instances[j])
			for id_algo in range(len(config.algos)):
				(mean,h,n) = mean_error(config, id_algo, j, config.max_seeds)
				f.write("%.0f " % mean)
				(mean,h,n) = mean_error_boxes(config, id_algo, j, config.max_seeds)
				f.write("%.0f " % mean)
				#~ f2.write("+- %.2f" % h)
				#~ f2.write(" (%d)\n" % n)
			f.write("\n")

	f.write("\n")
	f.write("gains\n")
	for id_mintime in range(len(config.mintimes)):
		f.write("%f " %config.mintimes[id_mintime])
		for id_algo in range(len(config.algos)):
			update_algo(config,id_algo, id_mintime)
			f.write("%f " % shared_gains[id_algo])
			f.write("%d " % shared_nb_comp[id_algo])
		f.write("\n")

	f.write("total time: ")
	for elem in shared_gains_ttime:
		f.write("%f " % elem)


	f.close()




def update_algo(config,id_algo, id_mintime=0):
	shared_nb_comp[id_algo] =0
	shared_L[id_algo] = 0
	shared_W[id_algo] = 0

	fL = open("%(dir)s/%(name)s_badresults.out" % {"dir":config.algos[id_algo].output_dir,"name":config.algos[id_algo].name}, "w")
	fW = open("%(dir)s/%(name)s_goodresults.out" % {"dir":config.algos[id_algo].output_dir,"name":config.algos[id_algo].name},  "w")
	for i in (k for k in range(len(config.instances)) if shared_times[id_algo,k] >= 0.0 and shared_times[0,k]>=0.0):
		if config.instances[i].endswith('*'): continue
		n=0.;sum0=0.;sum1=0.
		for kk in range(config.max_seeds):
			if shared_times[0,i+kk*len(config.instances)] >= 0.0 and shared_times[id_algo,i+kk*len(config.instances)]>=0.0:
				sum0+=shared_times[0,i+kk*len(config.instances)]
				sum1+=shared_times[id_algo,i+kk*len(config.instances)]
				n+=1.

		if n>0 and (sum0/n>config.mintimes[id_mintime] or sum1/n>config.mintimes[id_mintime]) and (sum0/n<config.maxtime or sum1/n<config.maxtime):
			if sum0/sum1 > float(config.sig_gain):
				shared_W[id_algo]+=1
				fW.write("%s %f %f\n" % (config.instances[i],sum1/n,sum0/n))
			if sum1/sum0 > float(config.sig_gain):
				shared_L[id_algo]+=1
				fL.write("%s %f %f\n" % (config.instances[i],sum1/n,sum0/n))
			shared_nb_comp[id_algo] +=1

	if shared_nb_comp[id_algo] > 0:
		av=av_rel_time(id_algo, config,id_mintime)
		shared_gains[id_algo]=(1.-av)/av
		shared_gains_ttime[id_algo]=gain_total_time(id_algo, config)
	else:
		shared_gains[id_algo]=1.0
		shared_gains_ttime[id_algo]=1.0

	fW.close()
	fL.close()

def update_all(config):

	for id_algo in range(len(config.algos)):
		update_share(config,id_algo,0)
		#~ update_algo(config,id_algo)


	#~ for id_inst in range(len(config.instances)):
		#~ update_inst(config,id_inst)

	print shared_gains
	print shared_run
	print shared_nb_comp



def update_share(config,id_algo,id_inst):
	#~ update_inst(config,id_inst)
	update_algo(config,id_algo)

	f = open("state.txt", "w")

	f.write("algos:")
	for elem in config.algos:
		f.write("%s " % elem.name)
	f.write("\n")

	f.write("gains (average relative times):")
	for elem in shared_gains:
		f.write("%f " % elem)
	f.write("\n")

	f.write("gains (total time)            :")
	for elem in shared_gains_ttime:
		f.write("%f " % elem)
	f.write("\n")

	f.write("compared instances:")
	for elem in shared_nb_comp:
		f.write("%3d " % elem)
	f.write("\n")

	f.write("runs    :")
	for elem in shared_run:
		f.write("%3d " % elem)
	f.write("\n")

	f.write("L:")
	for elem in shared_L:
		f.write("%3d " % elem)
	f.write("\n")

	f.write("W:")
	for elem in shared_W:
		f.write("%3d " % elem)
	f.write("\n")

	f.close()

def av_rel_time(i,config, id_mintime=0):
	if i==0: return 0.5
	x=[]
	for k in range(len(config.instances)):
		if config.instances[k].endswith('*'): continue
		n0=0;n1=0;sum0=0;sum1=0; sum2=0
		for kk in range(config.max_seeds):
			if shared_times[0,k+kk*len(config.instances)] >=0.0 and (shared_times[i,k+kk*len(config.instances)] >=0.0 or shared_times[i,k+(config.min_seeds-1)*len(config.instances)] >=0.0):
				sum0+=shared_times[0,k+kk*len(config.instances)]
				n0+=1
			if shared_times[i,k+kk*len(config.instances)] >=0.0 and (shared_times[0,k+kk*len(config.instances)] >=0.0 or shared_times[0,k+(config.min_seeds-1)*len(config.instances)] >=0.0):
				sum1+=shared_times[i,k+kk*len(config.instances)]
				n1+=1

		if n0>0 and n1>0 and (sum0/float(n0)>config.mintimes[id_mintime] or sum1/float(n1)>config.mintimes[id_mintime]) and (sum0/float(n0)<config.maxtime or sum1/float(n1)<config.maxtime):
			rel1 = (sum1/float(n1)) /(sum1/float(n1)+sum0/float(n0)+0.001)
			x.append(rel1)
	xmean=np.mean(x)
	return xmean

def gain_total_time(i,config):
	if i==0: return 1.0
	total_time0=0.
	total_time1=0.
	for k in range(len(config.instances)):
		if config.instances[k].endswith('*'): continue
		n0=0.; n1=0. ;sum0=0.;sum1=0.
		for kk in range(config.max_seeds):
			if shared_times[0,k+kk*len(config.instances)] >=0.0:
				sum0+=shared_times[0,k+kk*len(config.instances)]
				n0+=1.
			if shared_times[i,k+kk*len(config.instances)] >=0.0:
				sum1+=shared_times[i,k+kk*len(config.instances)]
				n1+=1.

		if n0>0. and n1>0. and ((sum0/n0)<config.maxtime or (sum1/n1)<config.maxtime):
			total_time0+=(sum0/n0)
			total_time1+=(sum1/n1)

	return total_time0/(total_time1+0.01)

def p_faster(i, j,config):
	if i==j: return 0.5
	x=[]
	for k in range(len(config.instances)):
		if config.instances[k].endswith('*'): continue
		n=0; sum0=0; sum1=0; sum2=0
		for kk in range(config.max_seeds):
			if shared_times[0,k+kk*len(config.instances)] >=0.0 and shared_times[i,k+kk*len(config.instances)] >=0.0 and shared_times[j,k+kk*len(config.instances)] >=0.0:
				sum0+=shared_times[0,k+kk*len(config.instances)]
				sum1+=shared_times[i,k+kk*len(config.instances)]
				sum2+=shared_times[j,k+kk*len(config.instances)]
				n+=1

		if n>0 and (sum0/float(n)>config.mintimes[0] or sum1/float(n)>config.mintimes[0] or sum2/float(n)>config.mintimes[0]) and (sum0/float(n)<config.maxtime or sum1/float(n)<config.maxtime or sum2/float(n)<config.maxtime):
			rel1=(sum1)/(sum1+sum0+0.001)
			rel2=(sum2)/(sum2+sum0+0.001)
			x.append(rel2-rel1)

	#x.append(1.0) #the best x for algo i (only for seed 1?), then worst_time/seed
	n= len(x)

	if n<=1: return 1.0

	return t.cdf((np.mean(x))/(np.std(x)/math.sqrt(n)),n-1)


def mean_error(config, id_algo, id_inst, seeds):
	x=[]
	for kk in range(seeds):
		if shared_times[id_algo,id_inst+kk*len(config.instances)] >=0.0:
			x.append(shared_times[id_algo,id_inst+kk*len(config.instances)])
	n=len(x)
	if n>=2:
		se=stats.sem(x)
		h = se*t._ppf((1+0.95)/2., n-1)
		return (np.mean(x),np.std(x),n)
	else:
		return (np.mean(x),-1,n)

def mean_error_boxes(config, id_algo, id_inst, seeds):
	x=[]
	for kk in range(seeds):
		if shared_boxes[id_algo,id_inst+kk*len(config.instances)] >=0.0:
			x.append(shared_boxes[id_algo,id_inst+kk*len(config.instances)])
	n=len(x)
	if n>=2:
		se=stats.sem(x)
		h = se*t._ppf((1+0.95)/2., n-1)
		return (np.mean(x),h,n)
	else:
		return (np.mean(x),-1,n)

def next_run(config):
	global lock
	global shared_stop

	lock.acquire()
	if shared_stop==1: return None

	for i in range(len(config.algos)):

		idx = shared_run[i]%len(config.instances)
		seed = int(shared_run[i]/len(config.instances)) + 1

		while shared_run[i] < len(config.instances)*config.max_seeds:

			if config.instances[config.idx2inst[idx]].endswith('*'):
				shared_run[i]+=1
				idx = shared_run[i]%len(config.instances)
				seed = int(shared_run[i]/len(config.instances)) + 1
				continue

			#en caso de que el algo_instancia se hayan corrido mas de min_seed veces, se calcula el diametro de confianza para el tiempo medio
			#si el diametro es < 0.1 no se sigue corriendo el par
			if seed>config.min_seeds and shared_times[i,len(config.instances)*(seed-1)+config.idx2inst[idx]] == -1.0:
				(mean,h,n)=mean_error(config,i,config.idx2inst[idx],seed-1)
				if mean>0:
					h/=mean
					if n>=config.min_seeds and h<0.1: shared_times[i,len(config.instances)*(seed-1)+config.idx2inst[idx]]=-1.6

			if shared_times[i,len(config.instances)*(seed-1)+config.idx2inst[idx]] != -1.0:
				shared_run[i]+=1
				idx = shared_run[i]%len(config.instances)
				seed = int(shared_run[i]/len(config.instances)) + 1
			else: break



	if shared_run.min() == len(config.instances)*config.max_seeds:
		lock.release()
		return None

	if shared_run[0]<shared_run.max():
		id_algo=0
	else:
		if shared_run.min() < 20:
			id_algo=shared_run.argmin()
		else:
			#se selecciona el algoritmo con menor tiempo promedio
			sorted_idx = sorted(range(len(config.algos)), key=lambda k: av_rel_time(k,config))
			print "gains:", shared_gains
			for id_algo in (j for j in sorted_idx if shared_run[j] < len(config.instances)*config.max_seeds):
				break

			#se calcula la  probabilidad, para cada algoritmo, de tener un tiempo promedio menor al seleccionado
			choices=[]
			for i in range(len(config.algos)):
				if shared_run[i] < len(config.instances)*config.max_seeds:
					choices.append(math.pow(p_faster(i,id_algo,config),1.0/4.0))
				else :
					choices.append(0.0)

			#se escoge usando la ruleta segun las probabilidades obtenidas
			print choices
			max=sum(choices)
			pick=random.uniform(0,max)
			current=0
			id_algo2=0
			for value in choices:
				current+=value
				if current>pick:
					break
				id_algo2+=1

			#el algoritmo con menor promedio es escogido si ha sido corrido un menor o igual numero de veces
			print config.algos[id_algo].name," ", config.algos[id_algo2].name
			if shared_run[id_algo2] < shared_run[id_algo]: id_algo=id_algo2





	idx = shared_run[id_algo]%len(config.instances)
	seed = int(shared_run[id_algo]/len(config.instances)) + 1
	shared_times[id_algo,len(config.instances)*(seed-1)+config.idx2inst[idx]]=-1.5
	shared_run[id_algo] += 1



	lock.release()

	inst=config.instances[config.idx2inst[idx]]
	(output_time, output_box, output) = config.algos[id_algo].run(inst,seed)

	lock.acquire()

	if shared_stop==1: return None

	shared_times[id_algo,len(config.instances)*(seed-1)+config.idx2inst[idx]] = output_time
	shared_boxes[id_algo,len(config.instances)*(seed-1)+config.idx2inst[idx]] = output_box

 	#the output file is updated
	import os.path
	if not os.path.isfile("%(dir)s/%(name)s_output.out" % {"dir":config.algos[id_algo].output_dir,"name":config.algos[id_algo].name}):
		open("%(dir)s/%(name)s_output.out" % {"dir":config.algos[id_algo].output_dir,"name":config.algos[id_algo].name}, "w")
 	lines = open("%(dir)s/%(name)s_output.out" % {"dir":config.algos[id_algo].output_dir,"name":config.algos[id_algo].name}).read().splitlines()
 	while len(lines)<len(config.instances):
 	 	lines.append( '' );
 	lines[config.idx2inst[idx]] = output
	open("%(dir)s/%(name)s_output.out" % {"dir":config.algos[id_algo].output_dir,"name":config.algos[id_algo].name}, "w").write('\n'.join(lines))

	print config.algos[id_algo].name, inst, output_time
	update_share(config,id_algo,len(config.instances)*(seed-1)+config.idx2inst[idx])
	write_times(config.algos[id_algo].name,id_algo,config)
	lock.release()


if __name__ == '__main__':
	config = Config(sys.argv[1])

	max_algos = len(config.algos)
	shared_times_base = multiprocessing.Array(ctypes.c_float, len(config.instances)*max_algos*config.max_seeds)
	shared_times = np.ctypeslib.as_array(shared_times_base.get_obj())
	shared_times = shared_times.reshape(max_algos, len(config.instances)* config.max_seeds)

	shared_boxes_base = multiprocessing.Array(ctypes.c_float, len(config.instances)*max_algos*config.max_seeds)
	shared_boxes = np.ctypeslib.as_array(shared_boxes_base.get_obj())
	shared_boxes = shared_boxes.reshape(max_algos, len(config.instances)* config.max_seeds)

	shared_gains_base = multiprocessing.Array(ctypes.c_float, max_algos)
	shared_gains = np.ctypeslib.as_array(shared_gains_base.get_obj())

	shared_gains_ttime_base = multiprocessing.Array(ctypes.c_float, max_algos)
	shared_gains_ttime = np.ctypeslib.as_array(shared_gains_ttime_base.get_obj())

	shared_run_base = multiprocessing.Array(ctypes.c_int, max_algos)
	shared_run = np.ctypeslib.as_array(shared_run_base.get_obj())

	shared_L_base = multiprocessing.Array(ctypes.c_int, max_algos)
	shared_L = np.ctypeslib.as_array(shared_L_base.get_obj())
	shared_W_base = multiprocessing.Array(ctypes.c_int, max_algos)
	shared_W = np.ctypeslib.as_array(shared_W_base.get_obj())
	shared_runinst_base = multiprocessing.Array(ctypes.c_int, len(config.instances))
	shared_runinst = np.ctypeslib.as_array(shared_runinst_base.get_obj())
	shared_nb_comp_base = multiprocessing.Array(ctypes.c_int, max_algos)
	shared_nb_comp = np.ctypeslib.as_array(shared_nb_comp_base.get_obj())
	shared_avgtime_base = multiprocessing.Array(ctypes.c_float, len(config.instances))
	shared_avgtime = np.ctypeslib.as_array(shared_avgtime_base.get_obj())
	shared_stddev_base = multiprocessing.Array(ctypes.c_float, len(config.instances))

	shared_stop = multiprocessing.Value('i', 0)

	i=0
	for algo in config.algos:
		read_times(config.algos[i].name,i,config)
		write_times(config.algos[i].name,i,config)
		i = i+1


	num_cores = int(sys.argv[2])
	pool = multiprocessing.Pool(num_cores)



	update_all(config)
	update_share(config,0,0)

	pool.map(next_run, itertools.repeat(config,len(config.instances)*config.max_seeds*len(config.algos)))

	i=0
	for algo in config.algos:
		write_times(config.algos[i].name,i,config)
		i = i+1

	update_all(config)


	final_tables(config)
