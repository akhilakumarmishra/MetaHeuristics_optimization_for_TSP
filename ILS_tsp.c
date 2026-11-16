#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define u01 rand()/(RAND_MAX + 1.0)
int cost[1000][1000];
int p[1000],x[1000],y[1000];
float pheromone_update[1000][1000];

int distance(int a1,int b1,int a2,int b2){
	int p=sqrt(( (a1 - b1)* (a1 - b1)) + ( (a2 - b2) * (a2 - b2)));
	return p;
}
int main()
{
	FILE *fp;
	fp=fopen("TSP-2.txt","r");
	int i,j,k;
//	i=j=k=0;
	int p[100],x[100],y[100];
	int c=0;
	
	i=0;
	fscanf(fp,"%d",&p[i]);
	fscanf(fp,"%d",&x[i]);
	fscanf(fp,"%d",&y[i]);
	i++;
	while(!feof(fp)){
		c++;
		fscanf(fp,"%d",&p[i]);
	        fscanf(fp,"%d",&x[i]);	
		fscanf(fp,"%d",&y[i]);
		i++;
	}
	for (i=0; i<c; i++) { p[i] = p[i] - 1; }
	
	fclose(fp);
	
        int cost[c][c];
        //cost matrix 
	for(i=0;i<c;i++){
		for(j=0;j<c;j++){
			cost[i][j]=distance(x[i],x[j],y[i],y[j]);
		}
	}
	int n = c;
	
	int visited[n],tour[n];
	for(j=0;j<n;j++){
		visited[j]= 0;
	}
	//initial solution generated
	for(i=0;i<n;i++){
		int rand_city = u01*n;
		while(visited[rand_city]==1){
						
			rand_city = u01*n;
		}
		visited[rand_city] = 1;
		tour[i] = rand_city;
		
	}
	int initial_cost=0;
	for( j=0;j<n;j++){
		if(j==n-1){
			initial_cost += cost[tour[j]][tour[0]];
		}
		else{
			initial_cost += cost[tour[j]][tour[j+1]];
		}
	}
	
	//2-opt local search
	int local_search_array[n];

	int total = 10000;
	int cx =0;
	int local_cost = initial_cost;
	int temp_array[n];
	int counter=0,value=initial_cost;
	while(total>=local_cost){
		for(j=0;j<n;j++){ 
//			printf("*a*\n");
			local_search_array[j]=tour[j];
		}
		int p,q;
		p= u01*n;
		q=u01*n;
		while(q == p ){
			q=u01*n;
		}
		//printf("\nlocalsearch:%d+%d\n",p,q);
		int tmp = local_search_array[p];
		local_search_array[p] = local_search_array[q];
		local_search_array[q] = tmp;
		
		total=0;
		for( j=0;j<n;j++){
			if(j==n-1){
				total += cost[local_search_array[j]][local_search_array[0]];
			}
			else{
				total += cost[local_search_array[j]][local_search_array[j+1]];
			}
		}
//		printf("AA%d\n",counter);
		if(total < local_cost){
//			printf("BB:>>%d\n",total);
			cx++;
			local_cost = total;
			value = local_cost;
			for(j=0;j<n;j++){
				temp_array[j] = local_search_array[j];
			}
		}		
		//printf("\nl_t_c:%d::I_B_C:%d\n",total,iteration_best_cost);	
		if(value == local_cost){
			counter ++;
		}
		else{
			printf("a\n");
			counter =0;
		}
		if(counter == 20000){
			break;
		}
		
	}
//	printf("cx:%d\n",cx);
//	printf("END details\n");
	float v;
	if(local_cost < initial_cost){
		initial_cost = local_cost;
		v= initial_cost;
		for(j=0;j<n;j++){
			tour[j] = temp_array[j];
//			printf("%d-->>>",tour[j]);
		}
//		printf("\nFINALcost:%d\n",initial_cost);
	}
	int itr=0,itr_counter;
	while(itr<20000){
		
		//PERTURBATION
		
		int p_tour[n];
		for(i=0;i<n;i++){
			p_tour[i] = tour[i];
		}
		int xp = 10;
		int temp[xp],temp_pos[xp],perturb_store[n];
		int visited_p[n];
		for(i=0;i<n;i++){
			visited_p[i] = 0;
		}
		for(i=0;i<xp;i++){
			temp[i] = u01* n;//select these positional values in tour[n]
			while(visited_p[temp[i]] == 1){
				temp[i] = u01 * n;
			}
			visited_p[temp[i]] =1;
	//		printf("%d ",temp[i]);
		}
		for(i=0;i<xp;i++){
			temp_pos[i] = p_tour[temp[i]];
			p_tour[temp[i]] = -1;
		}
	//	printf("\n");
	/*	for(i=0;i<xp;i++){
			printf("%d ",temp_pos[i]);
		}
	*/	int mn =n;
		k=0;
	//	printf("\n");
		while(k != xp){
			i=0;
			while(i<mn){
				if(p_tour[i] == -1){
	//				printf("%d\n",i);
					for(c = i; c < mn-1; c++){
						p_tour[c] = p_tour[c+1];
					}
					mn = mn-1;
					k++;
				}
				i++;
			}
		}
	/*	for(i=0;i<mn;i++){
				printf("%d  ",p_tour[i]);
		}
	*/
	//	printf("\n");	
		int min,tmp;
		for(j=0;j<xp;j++){
			int min =  cost[p_tour[0]][p_tour[1]];
			int pos = 1;
			for(i=1;i<mn-1;i++){
				tmp=cost[p_tour[i]][p_tour[i+1]];
				if(tmp < min){
					min = tmp;
					pos = i+1;
				}
				
			}
		//	printf("\n<%d:position:%d>\n",j,pos);
			mn=mn+1;
			for(i=0;i<pos;i++){
				perturb_store[i]=p_tour[i];
			}
			perturb_store[pos]= temp_pos[j];
			for(i=pos+1;i<mn;i++){
				perturb_store[i]=p_tour[i-1];
			}
			for(i=0;i<mn;i++){
				p_tour[i] = perturb_store[i];
			}
		/*	for(i=0;i<n;i++){
				printf("%d:>",perturb_store[i]);
			}*/
		}
	//	printf("\n\n\n");
	/*	for(i=0;i<n;i++){
			printf("%d:>",perturb_store[i]);
		}
	*/	
		//LOCAL SEARCH ON PERTURBATION
		
		int perturb_cost=0;
		for( j=0;j<n;j++){
			if(j==n-1){
				perturb_cost += cost[perturb_store[j]][perturb_store[0]];
			}
			else{
				perturb_cost += cost[perturb_store[j]][perturb_store[j+1]];
			}
		}
		int perturb_ls_array[n];
		total = 10000;
		local_cost = perturb_cost;
		int tmp_array[n];
		counter=0,
		value = perturb_cost;
		while(total>=local_cost){
			for(j=0;j<n;j++){ 
	//			printf("*a*\n");
				perturb_ls_array[j]=perturb_store[j];
			}
			int p,q;
			p= u01*n;
			q=u01*n;
			while(q == p ){
				q=u01*n;
			}
			
			int tmp = perturb_ls_array[p];
			perturb_ls_array[p] = perturb_ls_array[q];
			perturb_ls_array[q] = tmp;
			
			total=0;
			for( j=0;j<n;j++){
				if(j==n-1){
					total += cost[perturb_ls_array[j]][perturb_ls_array[0]];
				}
				else{
					total += cost[perturb_ls_array[j]][perturb_ls_array[j+1]];
				}
			}
		//	printf("AB%d\n",counter);
			if(total < local_cost){
		//		printf("BCB:>>%d\n",total);
				
				local_cost = total;
				value = local_cost;
				for(j=0;j<n;j++){
					tmp_array[j] = perturb_ls_array[j];
				}
				
			}		
			
			if(value == local_cost){
				counter ++;
			}
			else{
				printf("a\n");
				counter =0;
			}
			if(counter == 20000){
				break;
			}
			
		}
		
	//	printf("END details\n");
		if(local_cost < perturb_cost){
			perturb_cost = local_cost;
			
			for(j=0;j<n;j++){
				perturb_store[j] = tmp_array[j];
		//		printf("%d-->>>",perturb_store[j]);
			}
		//	printf("\nFINALcost:%d\n",initial_cost);
		}
		
		
		//acceptation_criteria
		if(perturb_cost< initial_cost){
			initial_cost = perturb_cost;
			v = initial_cost;
			for(i=0;i<n;i++){
				tour[i] = perturb_store[i];
			}
		}
		
		if(v == initial_cost){
			itr_counter++;
		}
		else{
			itr_counter =0;
		}
		if(perturb_cost >= initial_cost && itr_counter ==20){
			
			for(j=0;j<n;j++){
				visited[j]= 0;
			}
			//initial solution generated
			for(i=0;i<n;i++){
				int rand_city = u01*n;
				while(visited[rand_city]==1){
								
					rand_city = u01*n;
				}
				visited[rand_city] = 1;
				tour[i] = rand_city;
				
			}
			int initial_cost=0;
			for( j=0;j<n;j++){
				if(j==n-1){
					initial_cost += cost[tour[j]][tour[0]];
				}
				else{
					initial_cost += cost[tour[j]][tour[j+1]];
				}
			}
			
		}
		
	
		itr++;
	}
	
	printf("THE FINAL RESULT:\n");
	printf("cost:%d \n",initial_cost);
	printf("path:\n");
	for(i=0;i<n;i++){
		printf("%d ",tour[i]);
	}
	
}
