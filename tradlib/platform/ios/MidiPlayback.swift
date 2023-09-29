//
//  MidiPlayer.swift
//  TradApp
//
//  Created by damien murtagh on 9/28/23.
//

import Foundation

class MidiPlayback: ObservableObject {
    var avMidiPlayer: AVMIDIPlayer?
    var fileURL: URL?
    
    init(fileURL: URL) {
        self.fileURL = fileURL
        
        do {
            let audioSession = AVAudioSession.sharedInstance()
            try audioSession.setCategory(.playback, mode: .default)
            try audioSession.setActive(true)
        } catch {
            print("Setting up AVAudioSession failed: \(error)")
        }

    }
    
    func play() {
        
        // Create the audioPlayer
        if let fileURLUnwrapped = self.fileURL, let soundBankURL = Bundle.main.url(forResource: "FluidR3_GM", withExtension: "sf2") {
            do {
                self.avMidiPlayer = try AVMIDIPlayer(contentsOf: fileURLUnwrapped, soundBankURL: soundBankURL)
                self.avMidiPlayer?.prepareToPlay()
            } catch {
                NSLog("Error loading the audio file: \(String(describing: fileURL))\nError is: \(error)\nError local description: \(error.localizedDescription)")
            }
        }
        else {
            NSLog("File URL not set")
        }
        
        avMidiPlayer?.play()
    }
    
    func stop() {
        avMidiPlayer?.stop()
    }
}
