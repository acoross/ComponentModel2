using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class effect : MonoBehaviour {

    public float life = 1f;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        StartCoroutine("destroy");
	}

    IEnumerator destroy() {
        if (life > 0)
        {
            yield return new WaitForSeconds(life);
            Destroy(gameObject);
        }
    }
}
