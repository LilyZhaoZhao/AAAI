from numpy import *
import matplotlib.pylab as pl

def gibbs_sampler(alpha,delta,gamma,y,t):
    #initialize beta
    beta=1

    num_iter=100

    beta_draws=[]
    lambda_draws=[]

    for i in range(num_iter):
        #sample lambda given other lambdas and beta
        lambdas=lambda_update(alpha,beta,y,t)

        #record sample
        lambda_draws.append(lambdas)

        #sample beta given lambda samples
        beta=beta_update(alpha,gamma,delta,lambdas,y)

        #record sample
        beta_draws.append(beta)

    print beta_draws

    #pl.plot(beta_draws)
    #pl.show()

def lambda_update(alpha,beta,y,t):

    new_alpha=[(x+alpha) for x in y]
    new_beta=[1.0/(a+beta) for a in t]#Changed here

    #sample from this distribution 10 times
    samples=random.gamma(new_alpha,new_beta)
    return samples


def beta_update(alpha,gamma,delta,lambdas,y):
    #get sample
    sample=random.gamma(len(y)*alpha+gamma,
                        1.0 / (delta+sum(lambdas)))#Changed here
    return sample


def main():

    y=[5,1,5,14,3,19,1,1,4,22]
    t=[94,16,63,126,5,31,1,1,2,10]

    alpha=1.8
    gamma=0.01
    delta=1

    gibbs_sampler(alpha,delta,gamma,y,t)

if __name__ == '__main__':
    main()
