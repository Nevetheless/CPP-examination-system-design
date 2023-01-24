import jieba
from gensim.models import Word2Vec
import gensim.models.word2vec as w2v
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity
import argparse

def separateWords(filename):
    with open(filename, encoding='utf-8') as f:
        doc = f.read()
        doc_cut = jieba.cut(doc)
        result = ' '.join(doc_cut)
        with open('cpp简答题_seg.txt', 'w', encoding='utf-8') as f_seg:
            f_seg.write(result)

# 对句子中的每个词的词向量简单做平均 作为句子的向量表示
def wordavg(model,words): 
    res = np.zeros((1, 100))
    for word in words:
        try:
            res += np.array(model.wv[word]).reshape((1, 100))
        except KeyError as e:
            pass
    return np.mean(res,axis=0)

parser = argparse.ArgumentParser(description="Short Question Score")
parser.add_argument('--ans1', default='', type = str, help='input the ans1')
parser.add_argument('--student_ans1', default='', type = str, help='input the student_ans1')
parser.add_argument('--ans2', default='', type = str, help='input the ans2')
parser.add_argument('--student_ans2', default='', type = str, help='input the student_ans2')
parser.add_argument('--ans3', default='', type = str, help='input the ans3')
parser.add_argument('--student_ans3', default='', type = str, help='input the student_ans3')
parser.add_argument('--ans4', default='', type = str, help='input the ans4')
parser.add_argument('--student_ans4', default='', type = str, help='input the student_ans4')
parser.add_argument('--ans5', default='', type = str, help='input the ans5')
parser.add_argument('--student_ans5', default='', type = str, help='input the student_ans5')

args = parser.parse_args()

ans1 = args.ans1
ans2 = args.ans2
ans3 = args.ans3
ans4 = args.ans4
ans5 = args.ans5

student_ans1 = args.student_ans1
student_ans2 = args.student_ans2
student_ans3 = args.student_ans3
student_ans4 = args.student_ans4
student_ans5 = args.student_ans5

separateWords('D:\VisualStudio\QT\ExaminationSystem\word2vec\cpp简答题.txt')

model_name = 'cppShortQuestion.model'

##训练word2vec模型，生成词向量
sentences = w2v.LineSentence('cpp简答题_seg.txt')
model = Word2Vec(sentences, vector_size=100, window=5, min_count=5, workers=4, epochs=1000)
model.save(model_name)

a1_list = jieba.lcut(ans1, cut_all=True) # jieba分词
sa1_list = jieba.lcut(student_ans1, cut_all=True) # jieba分词
a1_avg = wordavg(model,a1_list) # s1的向量表示
sa1_avg = wordavg(model,sa1_list) # s2的向量表示
similarity1 = cosine_similarity(a1_avg.reshape(1,-1),sa1_avg.reshape(1,-1)) # 计算s1、s2向量的余弦相似度
print(similarity1[0][0])

a2_list = jieba.lcut(ans2, cut_all=True) # jieba分词
sa2_list = jieba.lcut(student_ans2, cut_all=True) # jieba分词
a2_avg = wordavg(model,a2_list) # s1的向量表示
sa2_avg = wordavg(model,sa2_list) # s2的向量表示
similarity2 = cosine_similarity(a2_avg.reshape(1,-1),sa2_avg.reshape(1,-1)) # 计算s1、s2向量的余弦相似度
print(similarity2[0][0])

a3_list = jieba.lcut(ans3, cut_all=True) # jieba分词
sa3_list = jieba.lcut(student_ans3, cut_all=True) # jieba分词
a3_avg = wordavg(model,a3_list) # s1的向量表示
sa3_avg = wordavg(model,sa3_list) # s2的向量表示
similarity3 = cosine_similarity(a3_avg.reshape(1,-1),sa3_avg.reshape(1,-1)) # 计算s1、s2向量的余弦相似度
print(similarity3[0][0])

a4_list = jieba.lcut(ans4, cut_all=True) # jieba分词
sa4_list = jieba.lcut(student_ans4, cut_all=True) # jieba分词
a4_avg = wordavg(model,a4_list) # s1的向量表示
sa4_avg = wordavg(model,sa4_list) # s2的向量表示
similarity4 = cosine_similarity(a4_avg.reshape(1,-1),sa4_avg.reshape(1,-1)) # 计算s1、s2向量的余弦相似度
print(similarity4[0][0])

a5_list = jieba.lcut(ans5, cut_all=True) # jieba分词
sa5_list = jieba.lcut(student_ans5, cut_all=True) # jieba分词
a5_avg = wordavg(model,a5_list) # s1的向量表示
sa5_avg = wordavg(model,sa5_list) # s2的向量表示
similarity5 = cosine_similarity(a5_avg.reshape(1,-1),sa5_avg.reshape(1,-1)) # 计算s1、s2向量的余弦相似度
print(similarity5[0][0])