import numpy as np


train_data1 = np.load('training_data-1.npy', allow_pickle=True)
train_data2 = np.load('training_data-2.npy', allow_pickle=True)
train_data3 = np.load('training_data-3.npy', allow_pickle=True)
train_data4 = np.load('training_data-4.npy', allow_pickle=True)
#train_data5 = np.load('training_data-5.npy', allow_pickle=True)
#train_data6 = np.load('training_data-6.npy', allow_pickle=True)
#train_data7 = np.load('training_data-7.npy', allow_pickle=True)
#train_data8 = np.load('training_data-8.npy', allow_pickle=True)
#train_data9 = np.load('training_data-9.npy', allow_pickle=True)
#train_data10 = np.load('training_data-10.npy', allow_pickle=True)
#train_data11 = np.load('training_data-11.npy', allow_pickle=True)
#train_data12 = np.load('training_data-12.npy', allow_pickle=True)
#train_data13 = np.load('training_data-13.npy', allow_pickle=True)


train_data =  np.concatenate((train_data1, train_data2, train_data3, train_data4))
                            #, train_data5, train_data6, train_data7,
                            #train_data8, train_data9, train_data10, train_data11, train_data12, train_data13))

print ("Train Data: ", train_data.shape)

file_name = '17June2021.npy'
np.save(file_name,train_data)
print('ALL DONE SAVED')
