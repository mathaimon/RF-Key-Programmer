<script setup>
import { ref } from 'vue';
import { vAutoAnimate } from '@formkit/auto-animate'

const isToggled = ref(false)
const transmitKey = ref()

function getRandomNumber(min, max) {
  transmitKey.value = Math.floor(Math.random() * (max - min + 1)) + min;
}

const toggleInstructions = ref(false)
</script>

<template>
  <div class="flex flex-col items-center px-5 pt-10 text-white">
    <h1 class="text-3xl font-semibold underline decoration-solid decoration-cyan-500">RF Key Programmer</h1>
    <div v-auto-animate="{ duration: 400 }"
      class="flex flex-col w-full max-w-md gap-3 p-6 mt-10 rounded-xl bg-neutral-700">
      <div class="text-xl font-semibold">RF Transmit Key</div>
      <input type="text" placeholder="696900" v-model="transmitKey"
        class="h-12 px-3 font-semibold border-2 rounded-md bg-neutral-800 bg-opacity-60 border-cyan-400">
      <div class="flex justify-between w-full gap-2">
        <button @click="getRandomNumber(11111215, 16777215)"
          class="w-full px-5 py-2 font-semibold duration-200 border-2 rounded-md min-h-12 bg-sky-600 border-sky-700 bg-opacity-30 hover:bg-sky-700">Generate
          Random
          Key</button>
        <button @click="isToggled = true"
          class="w-full px-5 py-2 font-semibold duration-200 bg-teal-600 border-2 border-teal-700 rounded-md min-h-12 bg-opacity-30 hover:bg-teal-700">Start
          Transmission</button>
      </div>
      <div v-if="isToggled" class="flex items-center gap-4 mx-auto mt-5">
        <div class="relative flex size-4">
          <span class="absolute w-full h-full border-2 rounded-full animate-ping bg-rose-600 border-rose-700">
          </span><span class="w-full h-full border-2 rounded-full anima bg-rose-600 bg-opacity-90 border-rose-700">
          </span>
        </div>
        <div>Transmitting Key : <span class="font-semibold">128754</span></div>
      </div>
      <button v-if="isToggled" @click="isToggled = false"
        class="px-5 py-2 mx-auto font-semibold duration-200 border-2 rounded-md min-h-12 bg-opacity-30 bg-rose-600 hover:bg-rose-700 border-rose-700">Stop
        Transmission</button>
      <div v-auto-animate class="p-3 rounded-md bg-neutral-800 bg-opacity-60">
        <div @click="toggleInstructions = !toggleInstructions" class="flex items-center font-semibold">
          <div
            class="flex items-center justify-center mr-3 bg-blue-500 bg-opacity-50 border-2 border-blue-500 rounded-full size-7">
            i</div>
          <span>Instructions</span>
        </div>
        <ul v-if="toggleInstructions" class="mt-3 ml-4 text-sm list-decimal list-outside">
          <li>Press both A and B buttons together for 3 seconds or until the light flashes.</li>
          <li>While holding the A button pressed press the B button thrice & release all buttons.</li>
          <li>Start transmission of new key.</li>
          <li>Press the key you want to program and move the remote near to the antenna of the RF Transmitter.</li>
        </ul>
      </div>
    </div>
  </div>
</template>

<style>
body {
  @apply bg-neutral-800
}
</style>