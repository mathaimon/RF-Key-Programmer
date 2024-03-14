<script setup>
import { ref } from 'vue';
import { vAutoAnimate } from '@formkit/auto-animate'

const isToggled = ref(false)
const transmitKey = ref()

function getRandomNumber(min, max) {
  transmitKey.value = Math.floor(Math.random() * (max - min + 1)) + min;
}
</script>

<template>
  <div class="flex flex-col items-center px-5 pt-10 text-white">
    <h1 class="text-3xl font-semibold underline decoration-solid decoration-cyan-500">RF Key Programmer</h1>
    <div v-auto-animate="{ duration: 400 }"
      class="flex flex-col w-full max-w-md gap-3 p-6 mt-10 rounded-xl bg-neutral-700">
      <div class="text-xl font-semibold">RF Transmit Key</div>
      <input type="text" placeholder="696900" v-model="transmitKey"
        class="h-12 px-3 font-semibold border rounded-md bg-neutral-800 bg-opacity-60 border-cyan-400">
      <div class="flex justify-between w-full gap-2">
        <button @click="getRandomNumber(11111215, 16777215)"
          class="w-full px-5 py-2 font-semibold duration-200 border-2 rounded-md min-h-12 bg-sky-600 border-sky-700 bg-opacity-30 hover:bg-sky-700">Generate
          Key</button>
        <button @click="isToggled = true"
          class="w-full px-5 py-2 font-semibold duration-200 border-2 rounded-md min-h-12 bg-opacity-30 bg-cyan-600 hover:bg-cyan-700 border-cyan-700">Start
          Transmission</button>
      </div>
      <div v-if="isToggled" class="flex items-center gap-4 mx-auto mt-5">
        <div class="border-2 rounded-full animate-ping size-5 bg-rose-600 bg-opacity-90 border-rose-700">
        </div>
        <div>Transmitting Key : <span class="font-semibold">128754</span></div>
      </div>
      <button v-if="isToggled" @click="isToggled = false"
        class="px-5 py-2 mx-auto font-semibold duration-200 border-2 rounded-md min-h-12 bg-opacity-30 bg-rose-600 hover:bg-rose-700 border-rose-700">Stop
        Transmission</button>
    </div>
  </div>
</template>

<style>
body {
  @apply bg-neutral-800
}
</style>