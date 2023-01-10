//
//  AudioPlayback.swift
//  Trad App
//
//  Created by damien murtagh on 12/30/22.
//

import Foundation
import AVFoundation

class AudioPlayback
{
    enum AudioPlaybackError: Error {
        case failedToPlayBuffer
        case failedToPlayFile
    }
    
    private let audioEngine = AVAudioEngine()
    private let playerNode = AVAudioPlayerNode()
    private let bus = 0
    
    func playAudioFile(_ filename: String, withExtension fileExtension: String) throws
    {
        // Get the path to the wav file
        let filePath = Bundle.main.path(forResource: filename, ofType: fileExtension)
        
        // Create the audio file URL
        guard let path = filePath else {
            print ("Error opening file")
            throw AudioPlaybackError.failedToPlayFile
        }
        let fileUrl = URL(fileURLWithPath: path)
        
        // Attach the player node to the audio engine
        audioEngine.attach(playerNode)
        
        // Connect the player node to the audio engine
        audioEngine.connect(playerNode, to: audioEngine.mainMixerNode, format: nil)
        
        // Start the audio engine
        try! audioEngine.start()
        
        // Load the audio file
        guard let audioFile = try? AVAudioFile(forReading: fileUrl) else
        {
            print("Error opening file")
            throw AudioPlaybackError.failedToPlayFile
        }
        print ("File Format \(audioFile)")
        
        // Schedule the audio file on the player node
        playerNode.scheduleFile(audioFile, at: nil)
        
        // Play the audio
        playerNode.play()
    }
    
    func playAudio(_ pcmBuffer: AVAudioPCMBuffer) throws
    {
        // Get the singleton instance for controlling the AVAudioSession
        let audioSession = AVAudioSession.sharedInstance()
        do
        {
            // Set the audio session category, mode, and options.
            try audioSession.setCategory(AVAudioSession.Category(rawValue: convertFromAVAudioSessionCategory(AVAudioSession.Category.playback)), mode: AVAudioSession.Mode.default)
        }
        catch
        {
            print("Failed to set audio session category.")
        }
        
        // Attach the player node to the audio engine.
        audioEngine.attach(playerNode)
        
        //let format = pcmBuffer.format
        let AVBufferSize = 1024
        
        let outputFormat = audioEngine.outputNode.outputFormat(forBus: 0)
        print ("Output Format \(outputFormat)")
        
        audioEngine.connect(playerNode, to: audioEngine.outputNode, format: pcmBuffer.format)
        playerNode.scheduleBuffer(pcmBuffer, completionHandler: {
            print("Finished Playing")
        })
        
        // Install tap here to draw the signal level and send current postioin of playing
//        audioEngine.outputNode.installTap(onBus: bus, bufferSize: AVAudioFrameCount(AVBufferSize), format: nil) {
//            buffer, time in
//            
//            DispatchQueue.main.async {
////                self.delegate?.audioSignal(didReceiveBuffer: buffer, timeStamp: time)
//                print("Tap dispatch async at time: \(time)")
//                
//            }
//        }
        
        do {
            try audioEngine.start()
            playerNode.play()
            print ("Playing audio")
        }
        catch {
            print ("\(error)")
            
            // Throw Exception
            throw AudioPlaybackError.failedToPlayBuffer
        }
    }
    
    // Helper function inserted by Swift 4.2 migrator.
    private func convertFromAVAudioSessionCategory(_ input: AVAudioSession.Category) -> String {
        return input.rawValue
    }
}
