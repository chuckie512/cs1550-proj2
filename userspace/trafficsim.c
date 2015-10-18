


void down(cs1550_sem *sem) {
  syscall(__NR_cs1550_down, sem);
}

void up(cs1550_sem *sem) {
  syscall(__NR_cs1550_down, sem);
}

int main(void){



  return 0;
}
