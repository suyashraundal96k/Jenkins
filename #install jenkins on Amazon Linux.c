# install jenkins on Amazon Linux
sudo wget -O /etc/yum.repos.d/jenkins.repo  https://pkg.jenkins.io/redhat-stable/jenkins.repo
sudo rpm --import https://pkg.jenkins.io/redhat-stable/jenkins.io-2023.key
sudo yum upgrade
sudo dnf install java-17-amazon-corretto -y
sudo yum install jenkins -y
sudo systemctl enable jenkins
sudo systemctl start jenkins
sudo systemctl status jenkins

# to resize temp file system to 1G
sudo mount -o remount,size=1G /tmp

#sudo yum install python3-pip -y
pip  -V


# test/test.py

import unittest
from app import app

class TestApp(unittest.TestCase):
    def test_home(self):
        tester = app.test_client()
        response = tester.get('/')
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.data, b"Python App Hosting")


if __name__ == "__main__":
    unittest.main()

# pythonapp build
 python3 -m venv myenv
source myenv/bin/activate
pip install -r requirements.txt
 

 #pythonapp test
 python3 -m venv myenv
source myenv/bin/activate
pip install -r requirements.txt
python3 -m unittest discover -s test

# pythonapp deploy (flask using gunicorn)
cd pythonapp
git pull origin master
pip install -r requirements.txt
killall gunicorn 
gunicorn --workers 3 --bind 0.0.0.0:5000 app:app&
 

# For Testing

 # pipeline {
    agent any
 
    stages {
        stage('git cloning') {
            steps {
                echo 'cloning files from github'
            }
        }
        stage('Build') {
            steps {
                echo 'Building nodejs project'
            }
        }
        stage('Test') {
            steps {
                echo 'Testing project'
            }
        }
        stage('Deploy') {
            steps {
                echo 'Deploying nodejs project on live server'
            }
        }


}

#For Building : 
pipeline {
    agent any
 
    stages {
        stage('git cloning') {
            steps {
                echo 'cloning files from github'

            }
        }
        stage('Build') {
            steps {
                echo 'Building nodejs project'
                sh 'npm install'
            }
        }
        stage('Test') {
            steps {
                echo 'Testing project'
            }
        }
        stage('Deploy') {
            steps {
                echo 'Deploying nodejs project on live server'
            }
        }
    }
}

# on liveserver
pipeline {
    agent any
     tools{
         nodejs 'mynode'
     }
     stages {
        stage('Git cloning') {
            steps {
                echo 'github checkout'
                checkout scmGit(branches: [[name: '*/master']], extensions: [], userRemoteConfigs: [[url: 'https://github.com/swati-zampal/nodepipeline.git']])
            }
        }
        stage('Build') {
            steps {
                echo 'Building Nodejs-app'
                sh "npm install"
            }
        }
        stage('Test') {
            steps {
                echo 'Testing'
                sh "./node_modules/mocha/bin/_mocha --exit ./test/test.js"
            }
        }
        stage('Deploy'){
            steps{
                echo "Deploying"
                }
            }
        }
    }
}


# Live server : 
pipeline {
    agent any
    tools{
        nodejs 'mynode'
    }
    stages {
        stage('git cloning') {
            steps {
                echo 'cloning files from github'
                checkout scmGit(branches: [[name: '*/master']], extensions: [], userRemoteConfigs: [[url: 'https://github.com/swati-zampal/nodepipeline.git']])
            }
        }
        stage('Build') {
            steps {
                echo 'Building nodejs project'
                sh 'npm install'
            }
        }
        stage('Test') {
            steps {
                echo 'Testing project'
                sh './node_modules/mocha/bin/_mocha --exit ./test/test.js'
            }
        }
        stage('Deploy') {
            steps {
                echo 'Deploying nodejs project on live server'
                script{
                    sshagent(['88b2758c-0f33-422e-b7e3-f148a49d58fb']) {  
                     sh '''
                           ssh -o StrictHostKeyChecking=no ubuntu@43.204.114.125<<EOF
                            cd /home/ubuntu/nodeapp/
                            git pull https://github.com/swati-zampal/nodepipeline.git
                            npm install
                            sudo npm install -g pm2
                            pm2 restart index.js || pm2 start index.js
		                    exit
                            EOF     
                           '''
                   }
                }
            }
        }
    }
}


