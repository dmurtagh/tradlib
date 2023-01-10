//
//  AudioCapture.swift
//  Trad App
//
//  Created by damien murtagh on 12/29/22.
//

import Foundation
import AVFoundation

class AudioCapture: NSObject, AVAudioRecorderDelegate
{
    private var recordingSession: AVAudioSession!
    private var audioRecorder: AVAudioRecorder!
    
    var audioRecorderCallback: ((_ success: Bool) -> Void)? = nil;
    
    // Tells the delegate when recording stops or finishes due to reaching its time limit.
    func audioRecorderDidFinishRecording(_ recorder: AVAudioRecorder, successfully flag: Bool)
    {
        print("audio Recorder Did Finish Recording with success flag: \(flag)")
        audioRecorderCallback?(flag)
    }
    
    // Tells the delegate that the audio recorder encountered an encoding error during recording.
    func audioRecorderEncodeErrorDidOccur(_ recorder: AVAudioRecorder, error: Error?)
    {
        print("audio Recorder Encode Error Did Occur")
        audioRecorderCallback?(false)
    }
    
    
    override init()
    {
        recordingSession = AVAudioSession.sharedInstance()
        super.init()
    }
    
    enum AudioCaptureError: Error
    {
        case failedToOpenFile(_ filename: String)
        case failedToReadIntoBuffer(_ filename: String)
        case recordPermissionsFailed
        case errorWhileRecording
    }
    
    func loadWavFile(_ filename: String) throws -> AVAudioPCMBuffer?
    {
        return try loadFile(filename, withExtension: "wav")
    }
    
    func loadFile(_ filename: String, withExtension fileExtension: String) throws -> AVAudioPCMBuffer?
    {
        print ("Loading: \(filename).\(fileExtension)")
        let fileURLOptional = Bundle.main.url(forResource: filename, withExtension: fileExtension)
        return try loadFile(fileURLOptional)
    }
   
    func loadFile(_ fileURLOptional: URL?) throws -> AVAudioPCMBuffer?
    {
        guard let unwrappedURL = fileURLOptional else
        {
            throw AudioCaptureError.failedToOpenFile(fileURLOptional!.description)
        }
        
        guard let audioFile = try? AVAudioFile(forReading: unwrappedURL, commonFormat: AVAudioCommonFormat.pcmFormatFloat32, interleaved: false) else
        //guard let audioFile = try? AVAudioFile(forReading: unwrappedURL, commonFormat: AVAudioCommonFormat.pcmFormatInt16, interleaved: true) else
        // Default (format not specified
        //guard let audioFile = try? AVAudioFile(forReading: unwrappedURL) else
        {
            throw AudioCaptureError.failedToOpenFile(fileURLOptional!.description)
        }
        
        print("Loading file of length: \(audioFile.length)")
        print("Format is \(audioFile.processingFormat.description)")
        
        let length = audioFile.length;
        
        guard let buffer = AVAudioPCMBuffer(pcmFormat: audioFile.processingFormat, frameCapacity: AVAudioFrameCount(audioFile.length)) else
        {
            throw AudioCaptureError.failedToReadIntoBuffer(fileURLOptional!.description)
        }
        
        do
        {
            try audioFile.read(into: buffer, frameCount: UInt32(length))
        }
        catch
        {
            throw AudioCaptureError.failedToReadIntoBuffer(fileURLOptional!.description)
        }
        
        return buffer
    }
    
    func startRecording(completionHandler: @escaping (_ success: Bool) -> Void) -> Void
    {
        audioRecorderCallback = completionHandler;
        do {
            try recordingSession.setCategory(.record, mode: .default)
            try recordingSession.setActive(true)
            recordingSession.requestRecordPermission() { [unowned self] allowed in
                DispatchQueue.main.async {
                    if allowed {
                        print ("Recording is allowed")
                        
                        // Start recording
                        let audioURL = Trad_App.AudioCapture.getRecordingURL()
                        print(audioURL.absoluteString)
                        
                        // Audio Settings
                        let settings = [
                            AVFormatIDKey: Int(kAudioFormatMPEG4AAC),
                            AVSampleRateKey: 44100,
                            AVNumberOfChannelsKey: 1,
                            AVEncoderAudioQualityKey: AVAudioQuality.high.rawValue
                        ]
                        
                        // Perform the recording
                        do {
                            // 5
                            self.audioRecorder = try AVAudioRecorder(url: audioURL, settings: settings)
                            self.audioRecorder.delegate = self
                            self.audioRecorder.record()
                        } catch {
                            completionHandler(false)
                        }
                        
                    } else {
                        print ("Recording not allowed")
                        
                        completionHandler(false)
                    }
                }
            }
        } catch {
            print ("\(error)")
            completionHandler(false)
        }
    }
    
    func stopRecording() {
        audioRecorder.stop()
        audioRecorder = nil
    }
    
    class func getDocumentsDirectory() -> URL {
        let paths = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)
        let documentsDirectory = paths[0]
        return documentsDirectory
    }

    class func getRecordingURL() -> URL {
        return getDocumentsDirectory().appendingPathComponent("tune.m4a")
    }
}

