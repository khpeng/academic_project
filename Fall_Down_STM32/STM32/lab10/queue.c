/*
 * 
 * Filename: queue.c

 * Author: Yifan Liu(liuyifa) Shengyu Chen(chen435) Shengwei Peng(pengs)
 * Maintainer: 
 * Created: Thu Feb  7 19:49:26 2013 (-0500)
 * Version: 
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * URL: 
 * Doc URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */



/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */

#include <queue.h>

void init_queue(queue_t *buf) {
  buf->head = 1; //1 denotes head
  buf->tail = 0; //0 denotes tail
  int i = 0;
  while( i<QUEUE_SIZE){//file buffer with 32 0's.
    buf->buffer[i]=0;
    i++;
  }
}

int enqueue (queue_t *buf, int data) {
  if(buf->head == buf->tail){
    return 0;}
  buf->buffer[buf->head] = data;
  buf->head = (buf->head+1)%QUEUE_SIZE;
  return 1;
}

int dequeue (queue_t *buf) {
  if(queue_empty(buf)){
    return 0;}
  int i = (buf->tail+1)%QUEUE_SIZE;
  int ans = buf->buffer[i];
  buf->tail = i;
  return ans;
}

int queue_empty(queue_t *buf) {
  /*if (buf->head == (buf->tail+1)%QUEUE_SIZE){
    return 1;}
  else {
  return 0;}*/
  int increased_tail = (buf->tail + 1) % QUEUE_SIZE;
  return increased_tail == buf -> head;
}


/* queue.c ends here */
