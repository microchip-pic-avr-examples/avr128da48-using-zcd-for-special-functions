// Jenkinsfile v1.2.0

pipeline {
    agent {
				label 'windows'
    }

    parameters {
        string( name: 'NOTIFICATION_EMAIL',
                defaultValue:  'PICAVR_Examples_GateKeepers@microchip.com',
                description: "Email to send build failure, fixed and successful deployment notifications.")
    }
	
	environment {
		GITHUB_OWNER = 'microchip-pic-avr-examples'
		GITHUB_URL ='https://github.com/microchip-pic-avr-examples/avr128da48-using-zcd-for-special-functions.git'
		BITBUCKET_URL = 'https://bitbucket.microchip.com/scm/ebe/avr128da48-using-zcd-for-special-functions.git'
		DEPLOY_TOOL_URL = 'https://bitbucket.microchip.com/scm/citd/tool-github-deploy.git'
		DEPLOY_SCRIPT_DIR = 'tool-github-deploy'
		DEPLOY_SCRIPT_FILE = 'deploy-source-as-is.sh'
	}

    options {
        timestamps()
        timeout(time: 30, unit: 'MINUTES')
    }

    stages {
        stage('Checkout') {
            steps {
				checkout scm
            }
        }
		
		stage('metadata') {
            steps {
				script {
					execute("pip install jsonschema")
					execute("git clone https://bitbucket.microchip.com/scm/citd/metadata-schema.git")						
					execute("git clone https://bitbucket.microchip.com/scm/citd/tool-metadata-validator.git")					
					execute("cd tool-metadata-validator && python metadata-validator.py -data ../.main-meta/main.json -schema ../metadata-schema/main-schema.json")
				}
            }
        }

		stage('Build') {
            steps {
				script {
					execute("git clone https://bitbucket.microchip.com/scm/citd/tool-studio-c-build.git")
					execute("cd tool-studio-c-build && python studiobuildtool.py")	
							
				}
            }
        }	
		
        stage('GitHub-Deploy') {
			when {
				not { 
					changeRequest() 
				}
				anyOf {
					tag ''
				}
            }
			steps {
				script {
					execute("git clone ${env.DEPLOY_TOOL_URL}")		
					
					withCredentials([usernamePassword(credentialsId: '8bit-examples.github.com', usernameVariable: 'USER_NAME', passwordVariable:'USER_PASS' )]) {					
						execute("cd ${env.DEPLOY_SCRIPT_DIR} && sh ${env.DEPLOY_SCRIPT_FILE} ${env.BITBUCKET_URL} ${env.GITHUB_URL} ${USER_NAME} ${USER_PASS} '--tag ${env.TAG_NAME}'")
					}	

                    sendSuccessfulGithubDeploymentEmail()					
				}
			}
        }

		stage('Portal-Deploy') {
			when {
				not { 
					changeRequest() 
				}			
				tag ''
			}
			steps {
				script {
					def metadata = readJSON file:".main-meta/main.json"					
					def version = metadata.content.version
					def project = metadata.content.projectName

					if(version == env.TAG_NAME) {				
						def cmdArgs = "'{\"repoOwnerName\":\"$env.GITHUB_OWNER\",\"repoName\":\"$project\",\"tagName\":\"$version\"}'"
						cmdArgs = cmdArgs.replaceAll("\"","\\\\\"")						
					
						execute("git clone https://bitbucket.microchip.com/scm/portal/bundles.git")						
						execute("git clone https://bitbucket.microchip.com/scm/citd/tool-portal-client-launcher.git")
						execute("cd tool-portal-client-launcher && node portalLauncher.js -app=../bundles/portal-client-cli-win.exe -cmd=\"uploadGitHub ${cmdArgs}\"")
						sendSuccessfulPortalDeploymentEmail()
					} else {
						echo "Tag name is not equal to metadata content version."
						execute("exit 1")
					}
					
				}
			}
		}
	}

    post {
        failure {			
            script {			
				sendPipelineFailureEmail()
            }
        }
        always {
            archiveArtifacts artifacts: "tool-studio-c-build/output/**", allowEmptyArchive: true, fingerprint: true
			script{
				cleanWs()
			}
        }
    }
}

def execute(String cmd) {
	if(isUnix()) {
		sh cmd
	} else {
		bat cmd
	}
}

def sendPipelineFailureEmail () {
    if (!"${env.CHANGE_AUTHOR_EMAIL}".equalsIgnoreCase("null")) {		  
        mail to: "${env.CHANGE_AUTHOR_EMAIL}, ${params.NOTIFICATION_EMAIL}",
        subject: "Failed Pipeline: ${currentBuild.fullDisplayName}",
        body: "Pipeline failure. ${env.BUILD_URL}"
    } else {			  
        mail to: "${params.NOTIFICATION_EMAIL}",
        subject: "Failed Pipeline: ${currentBuild.fullDisplayName}",
        body: "Pipeline failure. ${env.BUILD_URL}"
    }
}

def sendSuccessfulGithubDeploymentEmail () {
    mail to: "${params.NOTIFICATION_EMAIL}",
    subject: "Successful Github Deployment: ${currentBuild.fullDisplayName}",
    body: "The changes have been successfully deployed to GitHub. ${env.GITHUB_URL}"
}

def sendSuccessfulPortalDeploymentEmail () {
    mail to: "${params.NOTIFICATION_EMAIL}",
    subject: "Successful Portal Deployment: ${currentBuild.fullDisplayName}",
    body: "The changes have been successfully deployed to Discover Portal."
}