import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QIcon
import qdarkstyle
# from SignIn import SignInWidget
# from SignUp import SignUpWidget
# from SignUpTest import SignUpTestWidget
# import sip
# from AdminHome import AdminHome
# from StudentHome import StudentHome
# from changePasswordDialog import changePasswordDialog
# from utils.SocketClientThread import SocketClientThread
# from utils.SingletonQueue import SingletonQueue
# from utils.SingletonTcpSocket import SingletonTcpSocket

class Main(QMainWindow):
    def __init__(self, parent=None):
        super(Main, self).__init__(parent)
        self.layout = QHBoxLayout()
        self.widget = SignInWidget()
        self.resize(900, 600)
        self.setWindowTitle("欢迎登陆图书馆管理系统")
        self.setCentralWidget(self.widget)
        bar = self.menuBar()
        self.Menu = bar.addMenu("菜单栏")
        self.signUpAction = QAction("注册", self)
        self.signUpTestAction = QAction("注册用户", self)
        self.changePasswordAction =QAction("修改密码",self)
        self.signInAction = QAction("登录", self)
        self.quitSignInAction = QAction("退出登录", self)
        self.quitAction = QAction("退出", self)
        self.Menu.addAction(self.signUpAction)
        self.Menu.addAction(self.signUpTestAction)
        self.Menu.addAction(self.changePasswordAction)
        self.Menu.addAction(self.signInAction)
        self.Menu.addAction(self.quitSignInAction)
        self.Menu.addAction(self.quitAction)
        self.signUpAction.setEnabled(True)
        self.signUpTestAction.setEnabled(True)
        self.changePasswordAction.setEnabled(True)
        self.signInAction.setEnabled(False)
        self.quitSignInAction.setEnabled(False)
        self.widget.is_admin_signal.connect(self.adminSignIn)
        self.widget.is_student_signal[str].connect(self.studentSignIn)
        self.Menu.triggered[QAction].connect(self.menuTriggered)

    def adminSignIn(self):
        sip.delete(self.widget)
        self.widget = AdminHome()
        self.setCentralWidget(self.widget)
        self.changePasswordAction.setEnabled(False)
        self.signUpAction.setEnabled(True)
        self.signInAction.setEnabled(False)
        self.quitSignInAction.setEnabled(True)

    def studentSignIn(self, studentId):
        sip.delete(self.widget)
        self.widget = StudentHome(studentId)
        self.setCentralWidget(self.widget)
        self.changePasswordAction.setEnabled(False)
        self.signUpAction.setEnabled(True)
        self.signInAction.setEnabled(False)
        self.quitSignInAction.setEnabled(True)

    def menuTriggered(self, q):
        if(q.text()=="修改密码"):
            changePsdDialog=changePasswordDialog(self)
            changePsdDialog.show()
            changePsdDialog.exec_()
        if (q.text() == "注册"):
            sip.delete(self.widget)
            self.widget = SignUpWidget()
            self.setCentralWidget(self.widget)
            self.widget.student_signup_signal[str].connect(self.studentSignIn)
            self.signUpAction.setEnabled(False)
            self.changePasswordAction.setEnabled(True)
            self.signInAction.setEnabled(True)
            self.quitSignInAction.setEnabled(False)
        if (q.text() == "注册用户"):
            sip.delete(self.widget)
            self.widget = SignUpTestWidget()
            self.setCentralWidget(self.widget)
            self.widget.student_signup_signal[str].connect(self.studentSignIn)
            self.signUpAction.setEnabled(False)
            self.changePasswordAction.setEnabled(True)
            self.signInAction.setEnabled(True)
            self.quitSignInAction.setEnabled(False)
        if (q.text() == "退出登录"):
            sip.delete(self.widget)
            self.widget = SignInWidget()
            self.setCentralWidget(self.widget)
            self.widget.is_admin_signal.connect(self.adminSignIn)
            self.widget.is_student_signal[str].connect(self.studentSignIn)
            self.signUpAction.setEnabled(True)
            self.changePasswordAction.setEnabled(True)
            self.signInAction.setEnabled(False)
            self.quitSignInAction.setEnabled(False)
        if (q.text() == "登录"):
            sip.delete(self.widget)
            self.widget = SignInWidget()
            self.setCentralWidget(self.widget)
            self.widget.is_admin_signal.connect(self.adminSignIn)
            self.widget.is_student_signal[str].connect(self.studentSignIn)
            self.signUpAction.setEnabled(True)
            self.changePasswordAction.setEnabled(True)
            self.signInAction.setEnabled(False)
            self.quitSignInAction.setEnabled(False)
        if (q.text() == "退出"):
            qApp = QApplication.instance()
            qApp.quit()
        return


if __name__ == "__main__":
    # 连接服务器
    # socketThread = SocketClientThread('192.168.126.130', 9999)
    # socketThread.start()
    # SingletonQueue().put(['{"userId":"009","userPassword":"111sk0D83j&","userName":"我是谁1","userCreateTime":"2019/06/20 10:00:00"}', 1])
    # socketThread.join()
    SingletonTcpSocket().connect('192.168.126.130', 9999)

    app = QApplication(sys.argv)
    app.setWindowIcon(QIcon("./images/MainWindow_1.png"))
    app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt5())
    mainMindow = Main()
    mainMindow.show()
    sys.exit(app.exec_())
