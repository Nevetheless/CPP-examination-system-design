/*
SQLyog Professional v12.09 (64 bit)
MySQL - 8.0.24 : Database - examinationdb
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`examinationdb` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

USE `examinationdb`;

/*Table structure for table `choicebank` */

DROP TABLE IF EXISTS `choicebank`;

CREATE TABLE `choicebank` (
  `题号` int NOT NULL AUTO_INCREMENT,
  `题目类型` varchar(3) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT '选择题',
  `题目描述` varchar(100) COLLATE utf8mb4_general_ci NOT NULL,
  `选项A` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `选项B` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `选项C` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `选项D` varchar(50) COLLATE utf8mb4_general_ci NOT NULL,
  `答案` char(1) COLLATE utf8mb4_general_ci NOT NULL,
  `分值` int DEFAULT '2',
  PRIMARY KEY (`题号`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

/*Data for the table `choicebank` */

insert  into `choicebank`(`题号`,`题目类型`,`题目描述`,`选项A`,`选项B`,`选项C`,`选项D`,`答案`,`分值`) values (1,'选择题','1.下列关于new/delete和malloc/free的区别，描述错误的是（）？ ','A.用malloc需要指定内存分配的字节数且不能初始化对象，new会自动调用对象的构造函数','B.delete会调用对象的destructor，而free不会调用对象的destructor','C.malloc与free是标准库函数，new/delete是运算符','D.new、delete返回的是所分配变量（对象）的指针，malloc、free返回的是void指针','D',2),(2,'选择题','2.C++语言中，有关类的初始化叙述正确的是（）？','A.静态数据类型可以在类的外部进行初始化','B.可以在类定义时，对对象直接初始化','C.一般数据类型可以在类的外部进行初始化','D.静态函数中可以出现this指针','A',2),(3,'选择题','3.C++中不能够被派生类继承的有（）？','A.虚函数','B.纯虚函数','C.静态成员函数','D.构造函数','D',2),(4,'选择题','4.关于类模板的说法正确的是（）？','A.类模板的主要作用是生成抽象类','B.类模板实例化时，编译器根据给出的模板实参生成一个类','C.在类模板中的数据成员具有同样类型','D.类模板中的成员函数没有返回值','B',2),(5,'选择题','5.在C++语言中，以下说法正确的是（）？','A.多态是通过虚表实现的','B.构造函数可以声明为虚函数','C.析构函数不可以声明为虚函数','D.抽象类中至少包含一个纯虚函数','D',2),(6,'选择题','6.关于C++中的友元函数说法正确的是（）？','A.友元函数只能访问所在类的保护成员和公有成员，不能访问私有成员','B.友元函数是可以被继承的','C.友元函数没有this指针','D.友元函数破环了继承性机制','C',2),(7,'选择题','7.C++中，下列关于基类和派生类关系的叙述中，正确的是（）？','A.每个类最多只能有一个直接基类','B.派生类中的成员可以访问基类中的任何成员','C.基类的构造函数必须在派生类的构造函数体中进行显示调用','D.派生类除了继承基类的成员，还可以定义新的成员','D',2),(8,'选择题','8.有关运算符重载正确的描述是（）？','A.C++语言允许在重载运算符时改变运算符原来的功能','B.C++语言允许在重载运算符时改变运算符的优先级','C.C++语言允许在重载运算符时改变运算符的结合性','D.C++语言允许在重载运算符时改变运算符的操作个数','A',2),(9,'选择题','9.关于C++的inline关键字,以下说法正确的是（）？','A.使用inline关键字的函数会被编译器在调用处展开','B.头文件中可以包含inline函数的声明','C.可以在同一个项目的不同源文件内定义函数名相同但实现不同的inline函数','D.定义在Class声明内的成员函数默认是inline函数','D',2),(10,'选择题','10.在C++中，以下能对二维数组 a 进行正确初始化的语句是（）？','A.int a[2][]={{1,0,1},{5,2,3}};','B.int a[][]={{1,2,3},{4,5,6}};','C.int a[2][4]={1,2,3},{4,5},{6}};','D.int a[][3]={{1,0,1},{},{1,1}};','D',2);

/*Table structure for table `fillbank` */

DROP TABLE IF EXISTS `fillbank`;

CREATE TABLE `fillbank` (
  `题号` int NOT NULL AUTO_INCREMENT,
  `题目类型` varchar(3) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT '填空题',
  `题目描述` varchar(200) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `答案` varchar(100) COLLATE utf8mb4_general_ci NOT NULL,
  `空数` int NOT NULL,
  `分值` int NOT NULL,
  `无序` tinyint(1) NOT NULL,
  PRIMARY KEY (`题号`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

/*Data for the table `fillbank` */

insert  into `fillbank`(`题号`,`题目类型`,`题目描述`,`答案`,`空数`,`分值`,`无序`) values (1,'填空题','1.面向对象的三大特性为:_____、_____、_____。','继承 封装 多态',3,3,1),(2,'填空题','2.C语言结构化程序中，____是程序的基本组成单元，在C++面向对象设计框架中，____是程序的基本组成单元。','函数 类',2,2,0),(3,'填空题','3.类的私有成员只能被它的成员函数和____访问。','友元函数',1,1,0),(4,'填空题','4.C++类的数据成员和结构体的数据成员默认访问权限分别是____和____。','私有/private 公有/public',2,2,0),(5,'填空题','5.编译时的多态性通过____函数实现。','重载',1,1,0),(6,'填空题','6.在+、*、（）、>>运算符中，只能重载为成员运算符函数的有____。','()/（）',1,1,0),(7,'填空题','7.拷贝构造函数使用____作为参数来初始化创建中的对象。','同类型的对象 同类型对象 同类型的实例 同类型实例',1,1,0),(8,'填空题','8.____函数可以将一个指定的数据转换为类的对象，____函数实现类的对象转换成其他类型的数据。','转换构造 类型转换',2,2,0),(9,'填空题','9.#define DOUBLE(x) x + x;则i = 5 * DOUBLE(5)为____。','30/三十',1,1,0),(10,'填空题','10.当一个类A中没有任何成员变量和成员函数时，sizeof(A)为____。','1/一',1,1,0);

/*Table structure for table `programbank` */

DROP TABLE IF EXISTS `programbank`;

CREATE TABLE `programbank` (
  `题号` int NOT NULL AUTO_INCREMENT,
  `题目类型` varchar(3) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT '编程题',
  `题目描述` varchar(100) COLLATE utf8mb4_general_ci NOT NULL,
  `分值` int NOT NULL,
  PRIMARY KEY (`题号`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

/*Data for the table `programbank` */

insert  into `programbank`(`题号`,`题目类型`,`题目描述`,`分值`) values (1,'编程题','1.请用模板实现二维动态数组类，数组支持+、-运算符，并写出该类的测试代码。',10),(2,'编程题','2.编写一个函数模板，统计数组的平均值和标准差，并写出调用此函数的完整程序，使得函数调用时，数组的类型可以是整型也可以是双精度类型。',15),(3,'编程题','3.编写一个形状(Shape)类及其派生类，写出测试代码，需要体现出运行时多态。',15);

/*Table structure for table `shortbank` */

DROP TABLE IF EXISTS `shortbank`;

CREATE TABLE `shortbank` (
  `题号` int NOT NULL AUTO_INCREMENT,
  `题目类型` varchar(3) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT '简答题',
  `题目描述` varchar(100) COLLATE utf8mb4_general_ci NOT NULL,
  `答案` varchar(300) COLLATE utf8mb4_general_ci NOT NULL,
  `分值` int DEFAULT '5',
  PRIMARY KEY (`题号`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

/*Data for the table `shortbank` */

insert  into `shortbank`(`题号`,`题目类型`,`题目描述`,`答案`,`分值`) values (1,'简答题','1.请简述重载(overload)和重写(override)的区别？','重载：是指允许存在多个同名函数，而这些函数的参数表不同（或许参数个数不同，或许参数类型不同，或许两者都不同）。重写：是指子类重新定义父类虚函数的方法。',5),(2,'简答题','2.请简述C++中内存分配方式以及他们的区别？','1.从静态存储区域分配。内存在程序编译的时候就已经分配好，这块内存在程序的整个运行期间都存在。2.在栈上创建。在执行函数时，函数内局部变量的存储单元都可以在栈上创建，函数执行结束时这些存储单元自动被释放。3.从堆上分配，亦称动态内存分配。程序在运行的时候用malloc或new申请任意多少的内存，程序员自己负责在何时用free或delete释放内存。',5),(3,'简答题','3.请简要回答多态的作用？','1.应用程序不必为每一个子类编写功能调用，只需要对父类进行处理即可。2.大大提高程序的可复用性。',5),(4,'简答题','4.请说出const与#define相比具有哪些优点？','const作用：定义常量、修饰函数参数、修饰函数返回值三个作用。被const修饰的东西都受到强制保护，可以预防意外的变动，能提高程序的健壮性。',5),(5,'简答题','5.请简要说明模板的作用？','1.实现代码的可重用性。2.模板能减少源代码量，并提高代码的机动性，而不会降低类型安全。',5);

/*Table structure for table `studentinfo` */

DROP TABLE IF EXISTS `studentinfo`;

CREATE TABLE `studentinfo` (
  `学号` varchar(10) COLLATE utf8mb4_general_ci NOT NULL,
  `姓名` varchar(5) COLLATE utf8mb4_general_ci NOT NULL,
  `学院` varchar(10) COLLATE utf8mb4_general_ci NOT NULL,
  `班级` varchar(10) COLLATE utf8mb4_general_ci NOT NULL,
  `类别` varchar(5) COLLATE utf8mb4_general_ci NOT NULL,
  PRIMARY KEY (`学号`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

/*Data for the table `studentinfo` */

insert  into `studentinfo`(`学号`,`姓名`,`学院`,`班级`,`类别`) values ('M2022701','胡同学','机械科学与工程学院','机硕2210','硕士'),('M2022702','刘同学','电气与电子工程学院','电气2201','硕士'),('M2022703','陈同学','材料科学与工程学院','材料2202','硕士'),('M2022704','张同学','电子信息与通信学院','电子2203','硕士'),('M2022705','李同学','计算机科学与技术学院','计科2204','硕士'),('M2022706','王同学','人工智能与自动化学院','自动化2205','硕士'),('M2022707','赵同学','能源与动力工程学院','能源2206','硕士'),('M2022708','孙同学','土木与水利工程学院','工管2207','硕士'),('M202270809','吴同学','航空航天学院','航天2209','硕士'),('M2022709','周同学','软件学院','软工2208','硕士');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
